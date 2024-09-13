#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>


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
        std::cout << "<p>Aucune donnée reçue pour la suppression.</p>";
        return 0;
    }

    std::string nomEtudiant;
    const char* nom_cstr = std::strstr(query_string, "nom=");
    if (nom_cstr)
     {
        nom_cstr += std::strlen("nom=");
        std::string nom_str(nom_cstr);
        size_t pos = nom_str.find("&");
        if (pos != std::string::npos) 
        {
            nom_str = nom_str.substr(0, pos);
        }
        nomEtudiant = nom_str;
    }
    std::cout << "<p>" << nomEtudiant << "</p>";

    Etudiant etudiant;
    if (etudiant.supprimerEtudiant("dataEtudiant.csv", nomEtudiant))
     {
        std::cout << "<p>L'étudiant " << nomEtudiant << " a été supprimé avec succès.</p>";
    } 
    else 
    {
        std::cout << "<p>Erreur lors de la suppression de l'étudiant " << nomEtudiant << ".</p>";
    }

    std::cout << R"(<style>
                    a {
                        text-decoration: none;
                        color: white;
                    }
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
                   </style>
                   <button>
                    <a href="accueil.cgi">Retour à la liste des étudiants</a>
                   </button>)";

    return 0;
}
