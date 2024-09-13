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

int main() {
    std::cout << "Content-Type: text/html\n\n";

    // Récupérer le cookie 'session_id' s'il existe
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

    // Si la session est valide, continuer l'exécution normale du programme
    std::cout << R"(<!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Liste des Étudiants</title>
                        <style>
                            body {
                                font-family: Arial, sans-serif;
                                margin: 20px;
                                background-color: #f4f4f4;
                            }
                            table {
                                width: 100%;
                                border-collapse: collapse;
                                margin-bottom: 20px;
                            }
                            table, th, td {
                                border: 1px solid #ddd;
                            }
                            th, td {
                                padding: 8px;
                                text-align: left;
                            }
                            th {
                                background-color: #4CAF50;
                                color: white;
                            }
                            tr:nth-child(even) {
                                background-color: #f2f2f2;
                            }
                            form {
                                display: inline-block;
                                margin: 0;
                            }
                            a {
                                text-decoration: none;
                                color: #4CAF50;
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
                            button a {
                                color: white;
                                text-decoration: none;
                            }
                        </style>
                    </head>
                    <body>)" << std::endl;

    // Affichage de la liste des étudiants
    Etudiant etudiant;
    etudiant.afficherDonneesCSV("dataEtudiant.csv");

    // Bouton pour ajouter un étudiant
    std::cout << "<button><a href='formulaire.html'>AJOUTER</a></button>";

    // Bouton de déconnexion
    std::cout << "<button> <a href='logout.cgi'>Déconnexion</a></button>";

    std::cout << R"(</body>
                </html>)" << std::endl;
    
    return 0;
}
