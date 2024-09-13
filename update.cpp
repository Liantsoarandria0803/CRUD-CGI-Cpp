#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <regex>

bool validerNom(const std::string& nom) {
    std::regex nomPattern("^[A-Za-zÀ-ÿ '-]+$");
    return std::regex_match(nom, nomPattern);
}

bool validerPrenom(const std::string& prenom) {
    std::regex prenomPattern("^[A-Za-zÀ-ÿ '-]+$");
    return std::regex_match(prenom, prenomPattern);
}

bool validerAge(const std::string& age) {
    std::regex agePattern("^([1-9]|[1-9][0-9]|1[0-9]{2})$"); 
    return std::regex_match(age, agePattern);
}

bool validerAdresse(const std::string& adresse) {
    std::regex adressePattern("^[A-Za-z0-9À-ÿ ,'-]+$"); // Lettres, chiffres et certains caractères spéciaux
    return std::regex_match(adresse, adressePattern);
}

// Fonction pour extraire le cookie session_id du navigateur
std::string get_cookie(const std::string& cookie_name) {
    char* cookies = std::getenv("HTTP_COOKIE");
    if (cookies) {
        std::string cookies_str(cookies);
        
        std::size_t pos = cookies_str.find(cookie_name + "=");
        if (pos != std::string::npos) {
            pos += cookie_name.length() + 1; // Position après le signe "="
            std::size_t end_pos = cookies_str.find(";", pos); // Fin du cookie
            if (end_pos == std::string::npos) {
                end_pos = cookies_str.length(); // Si aucun autre cookie après, aller jusqu'à la fin
            }
            return cookies_str.substr(pos, end_pos - pos);
        }
    }
    return ""; // Retourner une chaîne vide si le cookie n'est pas trouvé
}

// Fonction pour vérifier si la session est valide
bool is_session_valid(const std::string& session_id) {
    std::ifstream session_file("sessions.txt");
    if (!session_file.is_open()) {
        return false;
    }
    
    std::string stored_session_id, login;
    while (session_file >> stored_session_id >> login) {
        if (stored_session_id == session_id) {
            return true;  // Session valide
        }
    }
    session_file.close();
    return false;  // Session invalide
}

int main() {
     std::cout << "Content-Type: text/html\n\n";

    // Vérifier la validité de la session
   std::string existing_session_id = get_cookie("session_id");
    
    // Vérifier la validité de la session
    if (existing_session_id.empty() || !is_session_valid(existing_session_id))  
    {
        std::cout << "<html>";
        std::cout << "<head>";
          std::cout << "<meta http-equiv='refresh' content='3;url=index.html'>";
        std::cout << "<title>Redirection</title>";
        std::cout << "</head>";
        std::cout << "<body>";
        std::cout << "<p>Session invalide ou non trouvée. Redirection vers la page de connexion...</p>";
        std::cout << "</body>";
        std::cout << "</html>";
        return EXIT_SUCCESS;
    }

    
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

    // Valider les données reçues
    if (!validerNom(nom)) {
        std::cout << "<p>Erreur : Nom invalide. Veuillez utiliser uniquement des lettres et des caractères autorisés.</p>";
        return 0;
    }
    if (!validerPrenom(prenom)) {
        std::cout << "<p>Erreur : Prénom invalide. Veuillez utiliser uniquement des lettres et des caractères autorisés.</p>";
        return 0;
    }
    if (!validerAge(std::to_string(age))) {
        std::cout << "<p>Erreur : Âge invalide. L'âge doit être un entier entre 1 et 199.</p>";
        return 0;
    }
    if (!validerAdresse(adress)) {
        std::cout << "<p>Erreur : Adresse invalide. Veuillez utiliser uniquement des lettres, chiffres et caractères autorisés.</p>";
        return 0;
    }

    // Mise à jour des données si toutes les validations passent
    Etudiant etudiant;
    if (etudiant.mettreAJourEtudiant("dataEtudiant.csv", old_nom, nom, prenom, age, adress)) {
        std::cout << "<p>Les données ont été mises à jour avec succès.</p>";
        std::cout << R"(<button><a href="accueil.cgi">AFFICHER LES DONNEES</a></button>)" << std::endl;
    } else {
        std::cout << "<p>Erreur lors de la mise à jour des données.</p>";
        std::cout << R"(<button><a href="accueil.cgi">AFFICHER LES DONNEES</a></button>)" << std::endl;
    }

    return 0;
}
