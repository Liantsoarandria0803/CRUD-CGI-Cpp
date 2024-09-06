#include "Crud.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>

int main()
{
    std::cout << "Content-Type: text/html\n\n";
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
                            button a{
                            color:white;
                            }
                        </style>
                    </head>
                    <body>)" << std::endl;

    Etudiant etudiant;
    etudiant.afficherDonneesCSV("dataEtudiant.csv");
    std::cout << "<button><a href='formulaire.html'>AJOUTER</button>";

    std::cout << R"(</body>
                </html>)" << std::endl;
    return 0;
}
