#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime> // Pour générer un identifiant de session

// Fonction pour générer un identifiant de session aléatoire
std::string generate_session_id() {
    const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string session_id;
    for (int i = 0; i < 16; ++i) {
        session_id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return session_id;
}

// Fonction pour extraire une valeur de la chaîne de requête
std::string extract_parameter(const std::string& query_string, const std::string& param_name) {
    size_t start = query_string.find(param_name + "=");
    if (start == std::string::npos) {
        return "";
    }
    start += param_name.length() + 1;
    size_t end = query_string.find("&", start);
    return query_string.substr(start, end - start);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "Content-Type: text/html\r\n";
    std::cout << "Cache-Control: no-cache\r\n"; // Assurez-vous que la réponse n'est pas mise en cache
    std::cout << "Pragma: no-cache\r\n"; // Pour les navigateurs plus anciens
    std::cout << "Expires: 0\r\n"; // Assurez-vous que la réponse est immédiate
    
    // Récupérer la chaîne de requête
    char* query_string = std::getenv("QUERY_STRING");
    if (!query_string) {
        std::cout << "<p>Aucune donnée reçue.</p>";
        return EXIT_SUCCESS;
    }

    std::string query(query_string);

    // Extraire les paramètres 'login' et 'pass'
    std::string login = extract_parameter(query, "login");
    std::string password = extract_parameter(query, "pass");

    if (login.empty() || password.empty()) {
        std::cout << "<p>Données de formulaire incomplètes.</p>";
        return EXIT_SUCCESS;
    }

    // Vérifier les identifiants dans la base de données
    std::ifstream database("basedeDonnee.txt");
    if (!database.is_open()) {
        std::cout << "<p>Erreur lors de l'ouverture de la base de données.</p>";
        return EXIT_FAILURE;
    }

    std::string db_login, db_password;
    bool found = false;
    while (database >> db_login >> db_password) {
        if (db_login == login && db_password == password) {
            found = true;
            break;
        }
    }
    database.close();

    if (found) {
        // Générer un identifiant de session unique
        std::string session_id = generate_session_id();

        // Envoi du cookie au client via l'en-tête HTTP
        std::cout << "Set-Cookie: session_id=" << session_id << "; HttpOnly; Path=/; Max-Age=3600\r\n";

        // Sauvegarder la session dans un fichier de session
        std::ofstream session_file("sessions.txt", std::ios::app);
        if (session_file.is_open()) {
            session_file << session_id << " " << login << "\n";
            session_file.close();
        }

        // Redirection avec l'en-tête HTTP
        std::cout << "Location: accueil.cgi\r\n";
        std::cout << "\r\n"; // Fin des en-têtes HTTP
    } else {
        // Redirection en cas d'échec
        std::cout << "Location: index.html\r\n";
        std::cout << "\r\n"; // Fin des en-têtes HTTP
    }

    return EXIT_SUCCESS;
}
