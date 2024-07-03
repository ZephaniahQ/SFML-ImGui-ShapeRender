#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "imgui.h"
#include "imgui-SFML.h"
#include <cstring>

enum{
    en_circle,
    en_rectangle
}typedef ShapeSelected;

class Shape{

public:
    //std::string m_name;
    std::string m_name;
    bool m_highlighted{false};
    float m_x;
    float m_y;
    float m_sx;
    float m_sy;
    float m_colorArr[3];
    sf::Text m_text;
    float m_textSize;
    bool m_drawShape{true};
    float m_scale{1};

    Shape(std::string name, float x, float y, float sx, float sy, int cR, int cG, int cB, sf::Text text, float& textSize)
        : m_name{name}, m_x{x}, m_y{y}, m_sx{sx}, m_sy{sy}, m_text{text}, m_textSize{textSize} {

            
            m_colorArr[0] = cR / 255.0f;
            m_colorArr[1] = cG / 255.0f;
            m_colorArr[2] = cB / 255.0f;   
        }

    ~Shape() {
    }

    const char* getName() const{
        return m_name.c_str();
    }

    void updateName(char * nameBuff){
        m_name = std::string(nameBuff);
    }

    void toggleDraw(){
        m_drawShape = !m_drawShape;
    }

    bool drawState(){
        return m_drawShape;
    }

    float* getColorArr(){
        return m_colorArr;
    }

    sf::Color getSfColor() {
        return sf::Color(m_colorArr[0] * 255, m_colorArr[1] * 255, m_colorArr[2] * 255);
    }

    void setHighlighted(bool highlighted) {
        m_highlighted = highlighted;
    }
};

class Circle : public Shape{
    float m_R;
    sf::CircleShape m_shape;

public:

    Circle(std::string name, float x, float y, float sx, float sy, int cR, int cG, int cB, float r, sf::Text text, float& textSize)
        : Shape(name, x, y, sx, sy, cR, cG, cB, text, textSize), m_R{r}, m_shape{sf::CircleShape(m_R)} {
        m_shape.setPosition(m_x, m_y);
        m_shape.setFillColor(getSfColor());
    }

    void checkBounds(int sWidth, int sHeight) {
        if (m_x <= 0 || m_x >= sWidth - m_shape.getLocalBounds().width) m_sx *= -1;
        if (m_y <= 0 || m_y >= sHeight - m_shape.getLocalBounds().height) m_sy *= -1;
    }

    void updateShape() {
        m_x += m_sx;
        m_y += m_sy;
        m_shape.setPosition(m_x, m_y);
        m_shape.setFillColor(getSfColor());
    }

    void updateText() {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        sf::Vector2f textOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        m_text.setOrigin(textOrigin);
        m_text.setPosition(m_shape.getPosition() + sf::Vector2f(m_R*m_scale, m_R*m_scale));  // Center of the circle
        m_text.setCharacterSize(m_textSize*m_scale);
    }
            
    void draw(sf::RenderWindow& window) {
        updateText();
        m_text.setString(m_name);
        if (m_scale != 0) {
            m_shape.setFillColor(getSfColor());
            if (m_highlighted) {
                m_shape.setOutlineThickness(4.f);
                m_shape.setOutlineColor(sf::Color::Yellow);
            } else {
                m_shape.setOutlineThickness(0.f);
            }
            window.draw(m_shape);
            window.draw(m_text);
        }
    }

    void checkScale(){
        if(m_scale!=1){
            m_shape = sf::CircleShape(m_R*m_scale);
            m_shape.setPosition(m_x, m_y);
        }
    }
};

class Rectangle : public Shape {
    float m_W;
    float m_H;
    sf::RectangleShape m_shape;

public:

    Rectangle(std::string name, float x, float y, float sx, float sy, int cR, int cG, int cB, float w, float h, sf::Text text, float& textSize)
        : Shape(name, x, y, sx, sy, cR, cG, cB, text, textSize), m_W{w}, m_H{h}, m_shape{sf::RectangleShape(sf::Vector2f(m_W, m_H))} {
        m_shape.setPosition(m_x, m_y);
        m_shape.setFillColor(getSfColor());
    }

    void checkBounds(int sWidth, int sHeight) {
        if (m_x <= 0 || m_x >= sWidth - m_shape.getLocalBounds().width) m_sx *= -1;
        if (m_y <= 0 || m_y >= sHeight - m_shape.getLocalBounds().height) m_sy *= -1;
    }

    void updateShape() {
        m_x += m_sx;
        m_y += m_sy;
        m_shape.setPosition(m_x, m_y);
        m_shape.setFillColor(getSfColor());
    }

    void updateText() {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        sf::Vector2f textOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        m_text.setOrigin(textOrigin);
        m_text.setPosition(m_shape.getPosition() + sf::Vector2f(m_W*m_scale / 2.0f, m_H*m_scale / 2.0f));  // Center of the rectangle
        m_text.setCharacterSize(m_textSize*m_scale);
    }

    void draw(sf::RenderWindow& window){
        updateText();
        m_text.setString(m_name);
        if (m_scale != 0) {
            m_shape.setFillColor(getSfColor());
            if (m_highlighted) {
                m_shape.setOutlineThickness(4.f);
                m_shape.setOutlineColor(sf::Color::Yellow); 
            } else {
                m_shape.setOutlineThickness(0.f);
            }
            window.draw(m_shape);
            window.draw(m_text);
        }
    }

    void checkScale(){
        if(m_scale!=1){
            m_shape = sf::RectangleShape(sf::Vector2f(m_W*m_scale, m_H*m_scale));
            m_shape.setPosition(m_x, m_y);
        }
    }
};

void loadConfig(const std::string& filename, std::vector<Rectangle>& v_rectangles, std::vector<Circle>& v_circles, int& sWidth, int& sHeight, sf::Text& text,float& textSize, sf::Font& font) {
    std::ifstream fin{filename};

    if (!fin) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string temp;

    while (fin >> temp) {
        if (temp == "Window") {
            fin >> sWidth >> sHeight;
            std::cout << "window: " << sWidth << " " << sHeight << std::endl;
        } else if (temp == "Circle") {
            std::string name;
            float x, y, sx, sy, radius;
            int R, G, B;
            fin >> name >> x >> y >> sx >> sy >> R >> G >> B >> radius;
            v_circles.push_back(Circle(name, x, y, sx, sy, R, G, B, radius, text, textSize));
            std::cout << "Added Circle: " << name << " " << x << " " << y << " " << sx << " " << sy << " " << R << " " << G << " " << B << " " << radius << std::endl;
        } else if (temp == "Rectangle") {
            std::string name;
            float x, y, sx, sy, w, h;
            int R, G, B;
            fin >> name >> x >> y >> sx >> sy >> R >> G >> B >> w >> h;
            v_rectangles.push_back(Rectangle(name, x, y, sx, sy, R, G, B, w, h, text, textSize));
            std::cout << "Added Rectangle: " << name << " " << x << " " << y << " " << sx << " " << sy << " " << R << " " << G << " " << B << " " << w << " " << h << std::endl;
        } else if (temp == "Font") {
            std::string fontname;
            int size, R, G, B;
            fin >> fontname >> textSize >> R >> G >> B;
            if (!font.loadFromFile(fontname)){
                std::cerr << "Error loading font: " << fontname << std::endl; 
            }else{
                std::cout<< "Loaded font:" << fontname << " successfully" << std::endl;
            }
            text.setFont(font);
            text.setFillColor(sf::Color(R,G,B));
            text.setCharacterSize(textSize);
        } else {
            std::cerr << "Unknown entry in file" << std::endl;
        }
    }

    fin.close();
}

void TextCentered(std::string text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text.c_str());
}

int main() {

    //Initing stuff and loading config:
    int sWidth{400}, sHeight{200};
    std::vector<Rectangle> v_rectangles;
    std::vector<Circle> v_circles;
    std::string filename = "config.txt";
    sf::Text text;
    float textSize;
    sf::Font font;
    char*nameBuff = new char[256]{"enter name"};
    loadConfig(filename, v_rectangles, v_circles, sWidth, sHeight, text, textSize, font);

    //ImGui UI logic stuff:
    bool shapeSelected{false};
    int selectedShape;
    Circle* selectedCirc;
    Rectangle* selectedRect;
    const float buttonWidth = 350.0f;

    // Create the window
    sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "My window");
    window.setFramerateLimit(60);

    //imgui stuff:
    ImGui::SFML::Init(window, false);
    // Font:
    ImGuiIO io = ImGui::GetIO();
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("font2.ttf", 24.0f);
    ImGui::SFML::UpdateFontTexture();

    sf::Clock deltaClock;

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {

            //process imgui events:
            ImGui::SFML::ProcessEvent(event);   

            // "Close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        //update imgui gui:
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Shapes Control Panel");

        

        if (!shapeSelected) {
                TextCentered("Loaded Shapes");
                for (auto& circle : v_circles) {
                    if (ImGui::Button(circle.getName(), ImVec2(buttonWidth, 0))) {
                        shapeSelected = true;
                        selectedShape = en_circle;
                        selectedCirc = &circle;
                    }
                }
                for (auto& rectangle : v_rectangles) {
                    if (ImGui::Button(rectangle.getName(), ImVec2(buttonWidth, 0))) {
                        shapeSelected = true;
                        selectedShape = en_rectangle;
                        selectedRect = &rectangle;
                    }
                }
        }
        else{
            if(selectedShape == en_circle){
                selectedCirc->setHighlighted(true);
                TextCentered(selectedCirc->getName());
                ImGui::Checkbox("Render Shape", &(selectedCirc->m_drawShape));
                ImGui::InputText("", nameBuff, sizeof(nameBuff));
                ImGui::SameLine();
                if(ImGui::Button("Set name")){
                    selectedCirc->updateName(nameBuff);
                    nameBuff = "enter name";
                }

                ImGui::SliderFloat("Size",&(selectedCirc->m_scale),0,4);
                ImGui::SliderFloat("Velocity X", &(selectedCirc->m_sx), -8,8);
                ImGui::SliderFloat("Velocity Y", &(selectedCirc->m_sy), -8,8);
                ImGui::ColorEdit3("Shape Color", selectedCirc->getColorArr());
                if(ImGui::Button("Back")){
                    shapeSelected = !shapeSelected;
                    selectedCirc->setHighlighted(false);
                }
            }else if(selectedShape == en_rectangle){  
                selectedRect->setHighlighted(true);  
                ImGui::Text(selectedRect->getName());
                ImGui::InputText("", nameBuff, sizeof(nameBuff));
                ImGui::SameLine();
                if(ImGui::Button("Set name")){
                    selectedRect->updateName(nameBuff);
                    nameBuff = "enter name";
                }

                ImGui::Checkbox("Show shape", &(selectedRect->m_drawShape));
                ImGui::SliderFloat("Size",&(selectedRect->m_scale),0,4);
                ImGui::SliderFloat("Velocity X", &(selectedRect->m_sx), -8,8);
                ImGui::SliderFloat("Velocity Y", &(selectedRect->m_sy), -8,8);
                ImGui::ColorEdit3("Shape Color", selectedRect->getColorArr());
                if(ImGui::Button("Back")){
                    shapeSelected = !shapeSelected;
                    selectedRect->setHighlighted(false); 
                }
            }
            
        }

        ImGui::End();

        window.clear(sf::Color::Black);

        // Update and draw circles
        for (auto& circle : v_circles) {
            if(circle.m_drawShape)
            {
                circle.checkScale();
                circle.checkBounds(sWidth, sHeight);
                circle.updateShape();
                circle.updateText();
                circle.draw(window);
            }
        }

        // Update and draw rectangles
        for (auto& rectangle : v_rectangles) {
            if(rectangle.m_drawShape)
            {
                rectangle.checkScale();
                rectangle.checkBounds(sWidth, sHeight);
                rectangle.updateShape();
                rectangle.updateText();
                rectangle.draw(window);
            }
        }

        //render imgui gui:
        ImGui::SFML::Render(window);    

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}







