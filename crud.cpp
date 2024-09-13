#include "Crud.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>

Etudiant::Etudiant() {}

void Etudiant::setNom(const char* query_string) {
    const char* nom_cstr = std::strstr(query_string, "nom=");
    if (nom_cstr) {
        nom_cstr += std::strlen("nom=");
        std::string nom_str(nom_cstr);
        size_t pos = nom_str.find("&");
        if (pos != std::string::npos) {
            nom_str = nom_str.substr(0, pos);
        }
        nom = nom_str;
    }
}

void Etudiant::setPrenom(const char* query_string) {
    const char* prenom_cstr = std::strstr(query_string, "prenom=");
    if (prenom_cstr) {
        prenom_cstr += std::strlen("prenom=");
        std::string prenom_str(prenom_cstr);
        size_t pos = prenom_str.find("&");
        if (pos != std::string::npos) {
            prenom_str = prenom_str.substr(0, pos);
        }
        prenom = prenom_str;
    }
}

void Etudiant::setAge(const char* query_string) {
    const char* age_cstr = std::strstr(query_string, "age=");
    if (age_cstr) {
        age_cstr += std::strlen("age=");
        std::string age_str(age_cstr);
        size_t pos = age_str.find("&");
        if (pos != std::string::npos) {
            age_str = age_str.substr(0, pos);
        }
        try {
            age = std::stoi(age_str);
        } catch (const std::exception&) {
            age = 0;
        }
    }
}

void Etudiant::setAdress(const char* query_string) {
    const char* address_cstr = std::strstr(query_string, "adress=");
    if (address_cstr) {
        address_cstr += std::strlen("adress=");
        std::string address_str(address_cstr);
        size_t pos = address_str.find("&");
        if (pos != std::string::npos) {
            address_str = address_str.substr(0, pos);
        }
        addresse = address_str;
    }
}

std::string Etudiant::getNom()
{
    return nom;
}
std::string Etudiant::getPrenom()
{
    return prenom;
}
int Etudiant::getAge()
{
    return age;
}
std::string Etudiant::getAdresse()
{
    return addresse;
}




bool Etudiant::sauvegarderDansFichier(const std::string& nomFichier) const {
    std::ofstream dataEtudiant(nomFichier, std::ios::app);
    if (!dataEtudiant) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return false;
    }
    dataEtudiant << nom << "," << prenom << "," << age << "," << addresse << std::endl;
    dataEtudiant.close();
    return true;
}

void Etudiant::afficherMessageSucces() const {
    std::cout << "<p>Les données ont été enregistrées avec succès.</p>";
}

void Etudiant::afficherDonneesCSV(const std::string& nomFichier) {
    std::ifstream dataEtudiant(nomFichier);
    if (!dataEtudiant) {
        std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
        return;
    }
    
    std::cout << "<table border='1'>"
              << "<tr><th>Nom</th><th>Prénom</th><th>Age</th><th>Adresse</th><th>Actions</th></tr>";
    std::string ligne;
    while (std::getline(dataEtudiant, ligne)) {
        std::cout << "<tr>";
        std::string cellule;
        std::istringstream stream(ligne);
        while (std::getline(stream, cellule, ',')) {
            std::cout << "<td>" << cellule << "</td>";
        }
        std::string nomEtudiant = ligne.substr(0, ligne.find(','));
        std::cout << "<td>"
                  << "<form action=\"update_form.cgi\" method=\"GET\" style=\"display:inline;\">"
                  << "<input type=\"hidden\" name=\"nom\" value=\"" << nomEtudiant << "\">"
                  << "<input type=\"submit\" value=\"Mettre à jour\">"
                  << "</form>"
                  << "<form action=\"delete.cgi\" method=\"GET\" style=\"display:inline;\">"
                  << "<input type=\"hidden\" name=\"nom\" value=\"" << nomEtudiant << "\">"
                  << "<input type=\"submit\" value=\"Supprimer\">"
                  << "</form>"
                  << "</td>";
        std::cout << "</tr>";
    }
    std::cout << "</table>";

    dataEtudiant.close();
}

bool Etudiant::supprimerEtudiant(const std::string& nomFichier, const std::string& nomEtudiant) {
    std::ifstream dataEtudiant(nomFichier);
    if (!dataEtudiant) {
        std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
        return false;
    }

    std::vector<std::string> lignes;
    std::string ligne;
    bool found = false;

    // Lire toutes les lignes dans le vecteur
    while (std::getline(dataEtudiant, ligne)) {
        if (ligne.find(nomEtudiant) == std::string::npos) {
            lignes.push_back(ligne); // Conserver les lignes non correspondantes
        } else {
            found = true; // Trouver la ligne correspondante
        }
    }

    dataEtudiant.close();

    if (found) {
        // Réécrire le fichier avec les lignes mises à jour
        std::ofstream outFile(nomFichier, std::ios::trunc);
        if (!outFile) {
            std::cerr << "Erreur lors de l'ouverture du fichier pour écriture." << std::endl;
            return false;
        }
        for (const auto& l : lignes) {
            outFile << l << std::endl;
        }
        outFile.close();
    }
    return found;
}


bool Etudiant::mettreAJourEtudiant(const std::string& nomFichier, const std::string& nomEtudiant, 
                                   const std::string& nouveauNom, const std::string& nouveauPrenom, 
                                   int nouvelAge, const std::string& nouvelleAdresse) {
    std::ifstream dataEtudiant(nomFichier);
    if (!dataEtudiant) {
        std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
        return false;
    }

    std::vector<std::string> lignes;
    std::string ligne;
    bool found = false;

    // Lire toutes les lignes dans le vecteur
    while (std::getline(dataEtudiant, ligne)) {
        if (ligne.find(nomEtudiant) != std::string::npos) {
            std::ostringstream oss;
            oss << nouveauNom << "," << nouveauPrenom << "," << nouvelAge << "," << nouvelleAdresse;
            lignes.push_back(oss.str());
            found = true;
        } else {
            lignes.push_back(ligne);
        }
    }

    dataEtudiant.close();

    if (found) {
        // Réécrire le fichier avec les lignes mises à jour
        std::ofstream outFile(nomFichier, std::ios::trunc);
        if (!outFile) {
            std::cerr << "Erreur lors de l'ouverture du fichier pour écriture." << std::endl;
            return false;
        }
        for (const auto& l : lignes) {
            outFile << l << std::endl;
        }
        outFile.close();
    }
    return found;
}
void Etudiant::setSession(std::string sess){
    session=sess;
}
std::string Etudiant::getSession(){
    return session;
}