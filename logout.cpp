#include <iostream>
#include <cstdlib>

int main() {
    // Effacer le cookie en réglant son expiration dans le passé
    std::cout << "Content-Type: text/html\r\n";
    std::cout << "Set-Cookie: session_id=; expires=Thu, 01 Jan 1970 00:00:00 GMT; path=/\r\n";
    
    // Redirection vers index.html
    std::cout << "Location: index.html\r\n";
    std::cout << "Content-Length: 0\r\n"; // Pour éviter un contenu vide non désiré
    std::cout << "\r\n"; // Fin des en-têtes HTTP

    return 0;
}
