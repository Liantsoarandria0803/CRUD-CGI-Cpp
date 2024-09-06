#include "Crud.hpp"
#include <iostream>
#include <cstdlib>

int main() 
{
    std::cout << "Content-Type: text/html\n\n";

    char* query_string = std::getenv("QUERY_STRING");
    if (!query_string) {
        std::cout << "<p>Aucune donnée reçue.</p>";
        return EXIT_SUCCESS;
    }
    
    Etudiant etudiant;
    etudiant.setNom(query_string);
    etudiant.setPrenom(query_string);
    etudiant.setAge(query_string);
    etudiant.setAdress(query_string);

    if (etudiant.sauvegarderDansFichier("dataEtudiant.csv")) {
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
    } else {
        std::cout << "<p>Données non enregistrées.</p>";
    }

    return EXIT_SUCCESS;
}
