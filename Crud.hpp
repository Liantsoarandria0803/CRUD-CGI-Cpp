#ifndef CRUD_HPP
#define CRUD_HPP

#include <string>

class Etudiant {
private:
    std::string nom;
    std::string prenom;
    int age;
    std::string addresse;

public:
    Etudiant();

    // Méthodes pour set les valeurs 
    void setNom(const char* query_string);
    void setPrenom(const char* query_string);
    void setAge(const char* query_string);
    void setAdress(const char* query_string);

    // Méthode pour sauvegarder les données de l'étudiant dans un fichier
    bool sauvegarderDansFichier(const std::string& nomFichier) const;

    // Méthode pour afficher un message de succès
    void afficherMessageSucces() const;

    // Méthode pour lire et afficher les données du fichier CSV
    void afficherDonneesCSV(const std::string& nomFichier);

    // Méthode pour supprimer un étudiant à partir d'un fichier CSV
    bool supprimerEtudiant(const std::string& nomFichier, const std::string& nomEtudiant);

    // Méthode pour mettre à jour les informations d'un étudiant dans un fichier CSV
    bool mettreAJourEtudiant(const std::string& nomFichier, const std::string& nomEtudiant, 
                             const std::string& nouveauNom, const std::string& nouveauPrenom, 
                             int nouvelAge, const std::string& nouvelleAdresse);
};

#endif
