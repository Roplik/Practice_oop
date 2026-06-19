#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void printUsage(const std::string& programName) 
{
    std::cout << "=== SFML Image Viewer ===" << std::endl;
    std::cout << "Usage: " << programName << " <path_to_image>" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " image.jpg" << std::endl;
    std::cout << "  " << programName << " C:/Users/User/Pictures/photo.png" << std::endl;
    std::cout << std::endl;
    std::cout << "Supported formats: PNG, JPG, BMP, TGA, etc." << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  ESC or Close window - Exit" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        printUsage(argv[0]);
        std::cout << "ERROR: No image path provided!" << std::endl;
        return 1;
    }

    std::string imagePath = argv[1];

    std::cout << "Loading image: " << imagePath << std::endl;

    sf::Texture texture;
    if (!texture.loadFromFile(imagePath)) 
    {
        std::cerr << "ERROR: Failed to load image: " << imagePath << std::endl;
        std::cerr << "Make sure the file is a valid image format." << std::endl;
        return 1;
    }
    
    sf::Sprite sprite(texture);
    
    sf::Vector2u textureSize = texture.getSize();
    std::cout << "Image loaded: " << textureSize.x << "x" << textureSize.y << " pixels" << std::endl;

    sf::VideoMode videoMode(sf::Vector2u(textureSize.x, textureSize.y));
    std::string windowTitle = "Image Viewer - " + fs::path(imagePath).filename().string();
    sf::RenderWindow window(videoMode, windowTitle, sf::Style::Default);
    window.setFramerateLimit(60);
    
    sprite.setPosition(sf::Vector2f(0, 0));
    
    std::cout << "Press ESC to exit" << std::endl;

    while (window.isOpen()) 
    {
        while (const auto event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape) 
                {
                    window.close();
                }
            }
        }
        
        window.clear(sf::Color(30, 30, 30));
        window.draw(sprite);
        window.display();
    }
    
    return 0;
}
