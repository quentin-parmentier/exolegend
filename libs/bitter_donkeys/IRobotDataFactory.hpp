#include <memory>


class IRobotDataFactory {
    public:
        virtual ~IRobotDataFactory() {}
    
        // Méthode pour créer un objet RobotData avec des propriétés prédéfinies
        virtual std::unique_ptr<RobotData> createRobotData(const std::string& name, int id) const = 0;
    
        // Méthode pour créer un objet RobotData aléatoire (généré à l'aide de distribution statistique)
        virtual std::unique_ptr<RobotData> createRandomRobotData() const = 0;
    
        // Méthode pour charger des données d'un fichier ou une base de données
        virtual std::unique_ptr<RobotData> loadRobotData(const std::string& filename) const = 0;
    };
    