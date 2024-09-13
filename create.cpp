#include "Crud.hpp"
#include <iostream>
#include <regex> 
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>

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

int main() 
{
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
    if (!query_string) 
    {
        std::cout << "<p>Aucune donnée reçue.</p>";
        return EXIT_SUCCESS;
    }
    
    // Extraire les valeurs
    Etudiant etudiant;
    etudiant.setNom(query_string);
    etudiant.setPrenom(query_string);
    etudiant.setAge(query_string);
    etudiant.setAdress(query_string);

    // Valider les données
    if (!validerNom(etudiant.getNom())|| !validerPrenom(etudiant.getPrenom()) || !validerAge(std::to_string(etudiant.getAge())) ||  !validerAdresse(etudiant.getAdresse())) {
        std::cout << "<p>Donnees invalide.</p>";
        std::cout << "<a href='formulaire.html'>Revenir</a>";
        return EXIT_SUCCESS;
    }

    // Sauvegarder les données si toutes les validations passent
    if (etudiant.sauvegarderDansFichier("dataEtudiant.csv"))
     {
        etudiant.afficherMessageSucces();
        std::cout << R"(<style>
                    button {
                        background-color: #4CAF50;
                        color: white;
                        padding: 10px;
                        border: none;
                        cursor: pointer;
                    }
                    button:hover {
                        background-color: #45a049;
                    }
                    a {
                        color: white;
                        text-decoration: none;
                    }
                   </style>
                   <button>
                    <a href="accueil.cgi">AFFICHER LES DONNEES</a>
                   </button>)" << std::endl;
    } 
    else 
    {
        std::cout << "<p>Données non enregistrées.</p>";
    }

    return EXIT_SUCCESS;
}
