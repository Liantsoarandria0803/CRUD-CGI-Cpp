#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

int main() {
    std::cout << "Content-Type: text/html\n\n";

    char* query_string = std::getenv("QUERY_STRING");
    if (!query_string) {
        std::cout << "<p>Aucune donnée reçue pour la mise à jour.</p>";
        return 0;
    }

    std::string old_nom, nom, prenom, adress;
    int age = 0;

    const char* old_nom_cstr = std::strstr(query_string, "old_nom=");
    if (old_nom_cstr) {
        old_nom_cstr += std::strlen("old_nom=");
        std::string old_nom_str(old_nom_cstr);
        size_t pos = old_nom_str.find("&");
        if (pos != std::string::npos) {
            old_nom_str = old_nom_str.substr(0, pos);
        }
        old_nom = old_nom_str;
    }

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

    const char* adress_cstr = std::strstr(query_string, "adress=");
    if (adress_cstr) {
        adress_cstr += std::strlen("adress=");
        std::string adress_str(adress_cstr);
        size_t pos = adress_str.find("&");
        if (pos != std::string::npos) {
            adress_str = adress_str.substr(0, pos);
        }
        adress = adress_str;
    }

    Etudiant etudiant;
    if (etudiant.mettreAJourEtudiant("dataEtudiant.csv", old_nom, nom, prenom, age, adress)) {
        std::cout << "<p>Les données ont été mises à jour avec succès.</p>";
            std::cout << R"(<button>
                        <a href="accueil.cgi">AFFICHER LES DONNEES</a>
                       </button>
        )" << std::endl;
    } else {
        std::cout << "<p>Erreur lors de la mise à jour des données.</p>";
            std::cout << R"(<button>
                        <a href="accueil.cgi">AFFICHER LES DONNEES</a>
                       </button>
        )" << std::endl;
    }

    return 0;
}
