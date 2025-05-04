// library
#include <SFML/Graphics.hpp>
#include <cmath>

// function - HSV to color
sf::Color HSV(unsigned int h, unsigned int s, unsigned int v)
{
    // constrain
    h %= 360;
    s %= 101;
    v %= 101;

    // normalize
    float H = h / 60.f;
    float S = s / 100.f;
    float V = v / 100.f;

    // index of hue
    uint8_t i = std::floor(H);

    // difference from index
    float D = H - i;

    // values
    float MAX = V;
    float MIN = V * (1 - S);
    float MID;
    if (i % 2 == 0)  MID = MIN + D*(MAX - MIN);
    else             MID = MAX + D*(MIN - MAX);

    // scale to 255
    uint8_t max = std::round(MAX * 255);
    uint8_t mid = std::round(MID * 255);
    uint8_t min = std::round(MIN * 255);

    // RGB
    uint8_t r, g, b;
    switch (i)
    {
        case 0:  r = max, g = mid, b = min;  break;
        case 1:  r = mid, g = max, b = min;  break;
        case 2:  r = min, g = max, b = mid;  break;
        case 3:  r = min, g = mid, b = max;  break;
        case 4:  r = mid, g = min, b = max;  break;
        case 5:  r = max, g = min, b = mid;  break;
    }

    return sf::Color(r, g, b);
}

// main program
int main()
{
    // HSV
    unsigned int h = 0;
    unsigned int s = 50;
    unsigned int v = 50;

    // create window
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Title");

    // set target frame rate
    window.setFramerateLimit(60);

    // create preview
    float preview_w = 200.f;
    sf::RectangleShape preview({preview_w, preview_w});
    preview.setOrigin({preview_w/2, preview_w/2});

    // create bar
    float bar_h = 20.f;
    sf::RectangleShape bar({preview_w, bar_h});
    bar.setOrigin({0.f, bar_h/2});

    // load font
    sf::Font font("C:/Windows/Fonts/arial.ttf");

    // create text
    sf::Text text(font);

    // lambda function - align text
    auto AlignText = [&] (float x, float y)
    {
        auto bounds = text.getLocalBounds();
        text.setOrigin({
            bounds.position.x + x*bounds.size.x,
            y*text.getCharacterSize()
        });
    };

    // window loop
    while (window.isOpen())
    {
        // handle events
        while (auto event = window.pollEvent())
        {
            // on close button release
            if (event->is<sf::Event::Closed>())
            {
                // close window
                window.close();
            }

            // when window is resized
            else if (auto resized = event->getIf<sf::Event::Resized>())
            {
                // update view
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
            }
        }

        // keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q) and h >   0) h -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::E) and h < 360) h += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) and s >   0) s -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) and s < 100) s += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Z) and v >   0) v -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::C) and v < 100) v += 1;

        // window size
        float window_w = window.getView().getSize().x;
        float window_h = window.getView().getSize().y;

        // fill background color
        window.clear(sf::Color(64, 64, 64));

        // draw preview
        auto color = HSV(h, s, v);
        preview.setFillColor(color);
        preview.setPosition({window_w/2, window_h/2});
        window.draw(preview);

        // draw HSV
        float x = window_w/2 - preview_w/2;
        float y = window_h/2 + preview_w/2 + 40.f;
        for (int i = 0;  i < 3;  i++)
        {
            float        scale;
            char         channel;
            unsigned int value;
            switch (i)
            {
                case 0: scale = h/360.f; channel = 'H'; value = h; break;
                case 1: scale = s/100.f; channel = 'S'; value = s; break;
                case 2: scale = v/100.f; channel = 'V'; value = v; break;
            }

            // bar
            bar.setPosition({x, y});
            bar.setFillColor(HSV(0, 0, 50));
            bar.setScale({1.f, 1.f});
            window.draw(bar);
            bar.setFillColor(HSV(0, 0, 100));
            bar.setScale({scale, 1.f});
            window.draw(bar);

            // channel
            text.setString(channel);
            text.setPosition({x - 20.f, y});
            AlignText(0.5f, 0.65f);
            window.draw(text);

            // value
            text.setString(std::to_string(value));
            text.setPosition({x + preview_w + 10.f, y});
            AlignText(0.f, 0.65f);
            window.draw(text);

            // move down
            y += 50.f;
        }

        // draw RGB
        uint8_t r = color.r;
        uint8_t g = color.g;
        uint8_t b = color.b;
        y = window_h/2 - preview_w/2 - 40.f;
        for (int i = 0;  i < 3;  i++)
        {
            float   scale;
            char    channel;
            uint8_t value;
            switch (i)
            {
                case 0: scale = b/255.f; channel = 'B'; value = b; break;
                case 1: scale = g/255.f; channel = 'G'; value = g; break;
                case 2: scale = r/255.f; channel = 'R'; value = r; break;
            }

            // bar
            bar.setPosition({x, y});
            bar.setFillColor(HSV(0, 0, 50));
            bar.setScale({1.f, 1.f});
            window.draw(bar);
            bar.setFillColor(HSV(0, 0, 100));
            bar.setScale({scale, 1.f});
            window.draw(bar);

            // channel
            text.setString(channel);
            text.setPosition({x - 20.f, y});
            AlignText(0.5f, 0.65f);
            window.draw(text);

            // value
            text.setString(std::to_string(value));
            text.setPosition({x + preview_w + 10.f, y});
            AlignText(0.f, 0.65f);
            window.draw(text);

            // move down
            y -= 50.f;
        }

        // display
        window.display();
    }

    // program end successfully
    return 0;
}