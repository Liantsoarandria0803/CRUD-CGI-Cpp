#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>

// Fonction pour extraire le cookie session_id du navigateur
std::string get_cookie_session_id() {
    char* cookies = std::getenv("HTTP_COOKIE");
    if (cookies) {
        std::string cookie_str(cookies);
        std::size_t pos = cookie_str.find("session_id=");
        if (pos != std::string::npos) {
            // Extraire la valeur du cookie session_id
            pos += std::strlen("session_id=");
            std::size_t end_pos = cookie_str.find(";", pos);
            return cookie_str.substr(pos, end_pos - pos);
        }
    }
    return "";
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

     // Récupérer le session_id depuis les cookies
    std::string session_id = get_cookie_session_id();
    
    // Vérifier la validité de la session
    if (session_id.empty() || !is_session_valid(session_id)) {
        // Redirection vers la page de connexion si la session n'est pas valide
        std::cout << "Status: 302 Found\n";
        std::cout << "Location: loginAccess.cgi\n\n";
        return EXIT_SUCCESS;
    }

    char* query_string = std::getenv("QUERY_STRING");
    if (!query_string) {
        std::cout << "<p>Aucune donnée reçue pour la mise à jour.</p>";
        return 0;
    }

    std::string nom;
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

    std::cout << R"(<!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Mise à jour des données</title>
                        <style>
                            body {
                                font-family: Arial, sans-serif;
                                margin: 20px;
                                background-color: #f4f4f4;
                            }
                            h1 {
                                color: #4CAF50;
                            }
                            form {
                                background-color: #fff;
                                padding: 20px;
                                border-radius: 5px;
                                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                                width: 500px;
                                margin: 20px auto;
                            }
                            label {
                                font-weight: bold;
                                margin-top: 10px;
                                display: block;
                            }
                            input[type="text"],
                            input[type="number"] {
                                width: 90%;
                                padding: 10px;
                                margin: 5px 0 20px 0;
                                border: 1px solid #ccc;
                                border-radius: 5px;
                            }
                            input[type="submit"] {
                                background-color: #4CAF50;
                                color: white;
                                padding: 10px 20px;
                                border: none;
                                border-radius: 5px;
                                cursor: pointer;
                                width: 100%;
                            }
                            input[type="submit"]:hover {
                                background-color: #45a049;
                            }
                        </style>
                    </head>
                    <body>
                    <h1>Mise à jour des informations de l'étudiant )" << nom << R"(</h1>
                    <form action="update.cgi" method="GET">
                        <input type="hidden" name="old_nom" value=")" << nom << R"(">
                        <label for="nom">Nom:</label><br>
                        <input type="text" id="nom" name="nom" value=")" << nom << R"("><br>
                        <label for="prenom">Prénom:</label><br>
                        <input type="text" id="prenom" name="prenom"><br>
                        <label for="age">Age:</label><br>
                        <input type="number" id="age" name="age"><br>
                        <label for="adress">Adresse:</label><br>
                        <input type="text" id="adress" name="adress"><br><br>
                        <input type="submit" value="Mettre à jour">
                    </form>
                    </body>
                    </html>)";
    return 0;
}
