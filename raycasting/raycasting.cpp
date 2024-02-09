#include <iostream>
#include "SFML/Graphics.hpp"
#include <cmath>

sf::Vector2f CheckIntersection(const sf::Vertex& a, const sf::Vertex& b, const sf::Vector2f& c, const sf::Vector2f& d)
{
 
    sf::Vector2f amc = sf::Vector2f(a.position.x - c.x, a.position.y - c.y);
    sf::Vector2f r = sf::Vector2f(b.position.x - a.position.x, b.position.y - a.position.y);
    sf::Vector2f s = d - c;
    float sxr = s.x * r.y - s.y * r.x;
    if (sxr == 0)
        return sf::Vector2f();
    float sxac = amc.x * s.y - amc.y * s.x;
    float rxac = amc.x * r.y - amc.y * r.x;

    float t = sxac / sxr;
    float u = rxac / sxr;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        return sf::Vector2f(a.position.x + t * r.x, a.position.y + t * r.y);
    }
    return sf::Vector2f();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 780), "My window");
    window.setMouseCursorVisible(false);
    sf::CircleShape dot(10.f);
    dot.setFillColor(sf::Color::Red);
    sf::ConvexShape shape1(3);
    shape1.setPoint(0, sf::Vector2f(200, 150));
    shape1.setPoint(1, sf::Vector2f(370, 120));
    shape1.setPoint(2, sf::Vector2f(220, 270));
    shape1.setFillColor(sf::Color::Transparent);
    shape1.setOutlineColor(sf::Color::Magenta);
    shape1.setOutlineThickness(2.f);
    sf::ConvexShape shape2(3);
    shape2.setPoint(0, sf::Vector2f(900.f, 180.f));
    shape2.setPoint(1, sf::Vector2f(850.f, 370.f));
    shape2.setPoint(2, sf::Vector2f(500.f, 200.f));
    shape2.setFillColor(sf::Color::Transparent);
    shape2.setOutlineColor(sf::Color::Magenta);
    shape2.setOutlineThickness(2.f);
    sf::ConvexShape shape3(4);
    shape3.setPoint(0, sf::Vector2f(200.f, 510.f));
    shape3.setPoint(1, sf::Vector2f(550.f, 470.f));
    shape3.setPoint(2, sf::Vector2f(500.f, 670.f));
    shape3.setPoint(3, sf::Vector2f(340.f, 690.f));
    shape3.setFillColor(sf::Color::Transparent);
    shape3.setOutlineColor(sf::Color::Magenta);
    shape3.setOutlineThickness(2.f);

    sf::ConvexShape shape4(3);
    shape4.setPoint(0, sf::Vector2f(900.f, 600.f));
    shape4.setPoint(1, sf::Vector2f(950.f, 700.f));
    shape4.setPoint(2, sf::Vector2f(850.f, 700.f));
    shape4.setFillColor(sf::Color::Transparent);
    shape4.setOutlineColor(sf::Color::Magenta);
    shape4.setOutlineThickness(2.f);


    sf::ConvexShape rect(4);
    rect.setPoint(0, sf::Vector2f(0,0));
    rect.setPoint(1, sf::Vector2f(window.getSize().x, 0));
    rect.setPoint(2, sf::Vector2f(window.getSize().x, window.getSize().y));
    rect.setPoint(3, sf::Vector2f(0, window.getSize().y));
    rect.setFillColor(sf::Color::Transparent);

    sf::VertexArray ray(sf::Lines, 2);
    
    std::vector<sf::ConvexShape> objects;
    objects.push_back(std::move(shape1));
    objects.push_back(std::move(shape2)); 
    objects.push_back(std::move(shape3));
    objects.push_back(std::move(shape4));
    objects.push_back(std::move(rect));

    sf::VertexArray fan(sf::TriangleFan);
    std::vector<sf::Vector2f> order; 
    

    while (window.isOpen())
    {   
        ray.clear();
        order.clear();
        fan.clear();
        double mouse_x = sf::Mouse::getPosition(window).x;
        double mouse_y = sf::Mouse::getPosition(window).y;
        fan.append(sf::Vector2f(mouse_x, mouse_y));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
      
     

        for (const auto& obj : objects)
        {
            for (int i = 0; i < obj.getPointCount(); i++)
            {
                float angle;
                double ray_x, ray_y;

                ray.append(sf::Vector2f(mouse_x, mouse_y));
                float target_x = obj.getPoint(i).x - mouse_x;
                float target_y = obj.getPoint(i).y - mouse_y;
                angle = std::atan2f(target_x, target_y);
                if (sqrtf(target_x * target_x + target_y * target_y) < 1800)
                {
                    ray.append(sf::Vector2f(obj.getPoint(i)));
                }
                else
                {
                       ray_x = mouse_x + 1800 * sin(angle);
                       ray_y = mouse_y + 1800 * cos(angle);
                      ray.append(sf::Vertex(sf::Vector2f(ray_x, ray_y) , sf::Color::White));
                }
               
                ray.append(sf::Vector2f(mouse_x, mouse_y));
                 ray_x = mouse_x + 1800 * sin(angle+0.0001);
                 ray_y = mouse_y + 1800 * cos(angle+0.0001);
                ray.append(sf::Vertex(sf::Vector2f(ray_x, ray_y), sf::Color::White));
              

                ray.append(sf::Vector2f(mouse_x, mouse_y));
                ray_x = mouse_x + 1800 * sin(angle - 0.0001);
                ray_y = mouse_y + 1800 * cos(angle - 0.0001);
                ray.append(sf::Vertex(sf::Vector2f(ray_x, ray_y), sf::Color::White));
            }
        }

        for (int i = 0; i < ray.getVertexCount(); i += 2)
        {
            sf::Vector2f closestIntersectionPoint;
            for (const auto& obj : objects)
            {
                for (int j = 0; j < obj.getPointCount(); j++)
                {
                     sf::Vector2f intersection = CheckIntersection(ray[i], ray[i+1], obj.getPoint(j), obj.getPoint((j + 1) % obj.getPointCount()));
                     if (intersection != sf::Vector2f())
                     {
                         if (closestIntersectionPoint == sf::Vector2f())
                         {
                             closestIntersectionPoint = intersection;
                         }
                         else
                         {
                             sf::Vector2f diffEX = sf::Vector2f(closestIntersectionPoint.x - ray[i].position.x, closestIntersectionPoint.y - ray[i].position.y);
                             float lEX = sqrtf(diffEX.x * diffEX.x + diffEX.y * diffEX.y);

                             sf::Vector2f diff = sf::Vector2f(intersection.x - ray[i].position.x, intersection.y - ray[i].position.y);
                             float l = sqrtf(diff.x * diff.x + diff.y * diff.y);

                             if (l < lEX)
                                 closestIntersectionPoint = intersection;
                         }
                     }
                }
            }
            if (closestIntersectionPoint != sf::Vector2f()) {
                ray[i + 1].position = closestIntersectionPoint;
                order.push_back(closestIntersectionPoint);
            }
          
        }
        sf::Vector2f center(mouse_x, mouse_y);
        std::sort(order.begin(), order.end(), [&](const sf::Vector2f& v1, const sf::Vector2f& v2) {
            sf::Vector2f vec1 = v1 - center;
            sf::Vector2f vec2 = v2 - center;
            return std::atan2(vec1.y, vec1.x) < std::atan2(vec2.y, vec2.x);
            });

        for (auto& ord : order)
        {
            fan.append(ord);
        }
        fan.append(order[0]);
        dot.setPosition(mouse_x , mouse_y);
        window.clear(sf::Color::Black);
      
        window.draw(ray);
        window.draw(fan);
        for (const auto& shape : objects)
        {
            window.draw(shape);
        }
        window.draw(dot);
        window.display();
    }
    return 0;
}
