#include "rainbow.h"
#include <unistd.h>
#include <regex>
#include <string>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

rainbow::rainbow(ledMatrix* leds, int sens): m_leds(leds), m_sens(sens), m_color(0)
{
    int saturation = 100;
    int lightness = 50;
    for (int i=0; i<180; i++)
    {
        int hue = i * 2;
        rainbowColors[i] = makeColor(hue, saturation, lightness);
    }
}

void rainbow::step()
{
    int x, y, index;
    for (x=0; x < LEDPERLINE; x++)
    {
        if(m_sens == VERTICAL)
        {
            index = (m_color + x*10) % 180;
        }
        for (y=0; y < NBLINES; y++)
        {
            if(m_sens == HORIZONTAL)
            {
                index = (m_color + y*5) % 180;
            }
            m_leds->setPixel(x, y, rainbowColors[index]);
        }
    }
    m_leds->update();
    usleep(60000);
    m_color = (m_color+1) % 180;
}

int rainbow::makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
    unsigned int red, green, blue;
    unsigned int var1, var2;

    if (hue > 359) hue = hue % 360;
    if (saturation > 100) saturation = 100;
    if (lightness > 100) lightness = 100;

    if (saturation == 0)
    {
        red = green = blue = lightness * 255 / 100;
    }
    else
    {
        if (lightness < 50)
        {
            var2 = lightness * (100 + saturation);
        }
        else
        {
            var2 = ((lightness + saturation) * 100) - (saturation * lightness);
        }
        var1 = lightness * 200 - var2;
        red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
        green = h2rgb(var1, var2, hue) * 255 / 600000;
        blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
    }
    return (red << 16) | (green << 8) | blue;
}

unsigned int rainbow::h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
    if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
    if (hue < 180) return v2 * 60;
    if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
    return v1 * 60;
}


std::string rainbow::get_page_name()
{
    return std::string("/rainbow.html");
}

void rainbow::get_page(std::stringstream& page_stream)
{
    page_stream.write((char*)header_page_html, header_page_html_len);
    page_stream << "        <div id=\"page-wrapper\">" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <h1 class=\"page-header\">Arc-en-ciel</h1>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <form id=\"form\" role=\"form\">" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Orientation</span>" << std::endl
                << "                <select name=\"orientation\" class=\"form-control\">" << std::endl
                << "                  <option";
    if(m_sens == HORIZONTAL)
        page_stream << " selected";
    page_stream << ">Horizontale</option>" << std::endl
                << "                  <option";
    if(m_sens == VERTICAL)
        page_stream << " selected";
    page_stream << ">Verticale</option>" << std::endl
                << "                </select>" << std::endl
                << "              </div>" << std::endl
                << "              <button id=\"submit\" type=\"submit\" class=\"btn btn-default\">Envoyer</button>" << std::endl
                << "            </form>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "      </div>" << std::endl;
    page_stream.write((char*)footer_page_html, footer_page_html_len);
}

void rainbow::set_parameters(char *params)
{
    rapidjson::Document d;
    d.Parse(params);
    rapidjson::Value& orient = d["orientation"];
    if(orient.IsString())
    {
        std::string orient_str = orient.GetString();
        if(orient_str == std::string("Verticale"))
        {
            m_sens = VERTICAL;
        }
        else if(orient_str == std::string("Horizontale"))
        {
            m_sens = HORIZONTAL;
        }
    }
    else
    {
        std::cout  << "Not Integer" << std::endl;
    }
}



