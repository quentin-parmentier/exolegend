#include "Navigation.hpp"

Navigation::Navigation(Gladiator *gladiator) : gladiator(gladiator)
{
    // On garde en mémoire la dernière cible qu'on nous a demandé d'atteindre pour recalculer notre point de départ
    startPosition = Vector2(0, 0);
    previousTargetPosition = Vector2(0, 0);
    lastState = NAVIGATION_TARGET_STATE::NOT_SET;
}

inline float Navigation::rotationOffsetFromAngleError(float angleOffset)
{
    // Seuil en dessous duquel on considère qu'on a pas besoin de corriger l'angle
    constexpr float ANGLE_REACHED_THRESHOLD = 2. * (M_PI / 180.f); // 2° de tolérance
    // Valeur d'angle en dessous de la quelle on applique une correction proportionnelle à la cible
    constexpr float MAX_ROTATION_SPEED = 0.20;
    float absAngleOffset = std::abs(angleOffset);
    // Si on a pas besoin de corriger l'angle
    if (absAngleOffset < ANGLE_REACHED_THRESHOLD)
    {
        return 0.;
        // Si on approche de la valeur à atteindre : on calcule une valeur de correction qui diminue lorsqu'on approche de la consigne
    }
    else
    {
        absAngleOffset = MAX_ROTATION_SPEED;
    }

    return angleOffset < 0 ? absAngleOffset : -absAngleOffset;
}

/**
 * @brief Calcule la vitesse à appliquer au robot en fonction de sa distance par rapport à la cible
 *
 * @param distanceFromStart la distance de départ de la cible pour calculer les rampe d'accélération et de décélération
 * @param distanceFromTarget
 * @return float
 */
inline float Navigation::speedValueFromDistance(float distanceFromStart, float distanceFromTarget)
{
    // Distance de décélération/d'accélération
    constexpr float slopDistance = 0.2;
    // Vitesse maximale absolue
    constexpr float maxSpeed = 0.4;
    // La plus petite vitesse qu'on doit envoyer aux moteurs pour que le robot se mette à bouger
    constexpr float minSpeed = 0.15;
    // Si la distance qu'on a à parcourir est inférieure à notre rampe d'accélération / décélération : on va à vitesse minimum
    if ((distanceFromStart + distanceFromTarget) < slopDistance)
    {
        return minSpeed;
        // Si on approche de la cible : on commence à décélérer
    }
    else if (distanceFromTarget < slopDistance)
    {
        return std::max(minSpeed, maxSpeed * (distanceFromTarget / slopDistance));
        // Si on est très loin de la cible
    }
    else
    {
        // Si on est en phase d'accélération
        if (distanceFromStart < slopDistance)
        {
            return std::max(minSpeed, maxSpeed * (distanceFromStart / slopDistance));
            // Si on a fini d'accélérer
        }
        else
        {
            return maxSpeed;
        }
    }
}

NAVIGATION_TARGET_STATE Navigation::driveTo(const Vector2 &target, bool showLogs)
{
    // Position actuelle du robot
    Position posRaw = gladiator->robot->getData().position;
    Vector2 pos{posRaw.x, posRaw.y};

    // Si on a changé de cible, on garde ne mémoire notre point de départ et notre nouvelle cible
    if (previousTargetPosition != target)
    {
        previousTargetPosition = Vector2(target.x(), target.y());
        lastState = NAVIGATION_TARGET_STATE::NOT_SET;
        startPosition = Vector2(pos.x(), pos.y());
        rotationCounter = 0.;
        previousPosition = Vector2(pos.x(), pos.y());
        if (showLogs)
        {
            gladiator->log("New target x %f, y %f ; current pos x %f, y %f", previousTargetPosition.x(), previousTargetPosition.y(), startPosition.x(), startPosition.y());
        }
    }
    else if (lastState == NAVIGATION_TARGET_STATE::REACHED)
    {
        return lastState;
    } else {
        rotationCounter += previousPosition.angle() - pos.angle();
    }

    Vector2 posError = target - pos;
    float distanceFromStart = (startPosition - pos).norm2();
    float distanceFromTarget = posError.norm2();

    float targetAngle = posError.angle();
    float angleError = moduloPi(targetAngle - posRaw.a);

    bool targetReached = false;

    // On calcule la vitesse max qu'on peut appliquer en fonction de la distance du point d'arrivée
    float speedValue = speedValueFromDistance(distanceFromStart, distanceFromTarget);

    // On calcule la rotation qu'on doit appliquer en fonction de l'angle qu'on a par rapport à l'arrivée
    float rotationOffset = rotationOffsetFromAngleError(angleError);

    // Calcul des commandes moteur
    float leftCommand = speedValue + rotationOffset;
    float rightCommand = speedValue - rotationOffset;

    // Analyse de la progression du robot par rapport à la cible
    if (distanceFromTarget < 0.03)
    if (distanceFromTarget < 0.03 || (abs(rotationCounter) > (2.*M_PI)))
    {
        lastState = NAVIGATION_TARGET_STATE::REACHED;
        leftCommand = 0.;
        rightCommand = 0.;
    }
    else if (distanceFromTarget < 0.1)
    {
        lastState = NAVIGATION_TARGET_STATE::CLOSE;
    }
    else
    {
        lastState = NAVIGATION_TARGET_STATE::FAR;
    }

    // écriture des commandes
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, leftCommand);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, rightCommand);
    if (showLogs || targetReached)
    {
        gladiator->log("ta %f, ca %f, ea %f, tx %f cx %f ex %f ty %f cy %f ey %f lc %f rc %f sd %f td %f", targetAngle, posRaw.a, angleError, target.x(), pos.x(), posError.x(), target.y(), pos.y(), posError.y(), leftCommand, rightCommand, distanceFromStart, distanceFromTarget);
    }
    return lastState;
}