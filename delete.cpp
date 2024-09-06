#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

int main() {
    std::cout << "Content-Type: text/html\n\n";

    char* query_string = std::getenv("QUERY_STRING");
    if (!query_string) {
        std::cout << "<p>Aucune donnée reçue pour la suppression.</p>";
        return 0;
    }

    std::string nomEtudiant;
    const char* nom_cstr = std::strstr(query_string, "nom=");
    if (nom_cstr) {
        nom_cstr += std::strlen("nom=");
        std::string nom_str(nom_cstr);
        size_t pos = nom_str.find("&");
        if (pos != std::string::npos) {
            nom_str = nom_str.substr(0, pos);
        }
        nomEtudiant = nom_str;
    }
    std::cout << "<p>" << nomEtudiant << "</p>";

    Etudiant etudiant;
    if (etudiant.supprimerEtudiant("dataEtudiant.csv", nomEtudiant)) {
        std::cout << "<p>L'étudiant " << nomEtudiant << " a été supprimé avec succès.</p>";
    } else {
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
