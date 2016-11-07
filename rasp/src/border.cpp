#include <regex>
#include <iomanip>
#include "border.h"
#include "unistd.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

border::border(ledMatrix* leds): m_leds(leds), m_color(WHITE)
{
}

void border::step()
{
    m_leds->clean();
    for(int i = 0; i < LEDPERLINE; i++)
    {
        m_leds->setPixel(i, 0, m_color);
        m_leds->setPixel(i, NBLINES-1, m_color);
    }
    for(int i = 0; i < NBLINES; i++)
    {
        m_leds->setPixel(0, i, m_color);
        m_leds->setPixel(LEDPERLINE-1, i, m_color);
    }
    m_leds->update();
}

std::string border::get_page_name()
{
    return std::string("/border.html");
}

void border::set_parameters(char* json)
{
    rapidjson::Document d;
    d.Parse(json);
    rapidjson::Value& color = d["color"];
    if(color.IsString())
    {
        std::string color_str = color.GetString();
        std::regex exp ("#([0-9a-fA-f]{6})");
        std::cmatch cm;
        std::regex_match(color_str.c_str(), cm, exp, std::regex_constants::match_default);
        if(cm.size() == 2)
        {
            m_color =  std::stoi(cm[1], nullptr, 16);
        }
    }
}

void border::get_page(std::stringstream& page_stream)
{
    page_stream.write((char*)header_page_html, header_page_html_len);
    page_stream << "        <div id=\"page-wrapper\">" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <h1 class=\"page-header\">Bordure de table</h1>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <form id=\"form\" role=\"form\">" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Couleur</span>" << std::endl
                << "                <input type=\"color\" name=\"color\" class=\"form-control\" value=\"#"<< std::hex << std::setw(6) << std::setfill('0') <<  m_color << std::dec <<"\">" << std::endl
                << "              </div>" << std::endl
                << "              <button id=\"submit\" type=\"submit\" class=\"btn btn-default\">Envoyer</button>" << std::endl
                << "            </form>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "      </div>" << std::endl;
    page_stream.write((char*)footer_page_html, footer_page_html_len);
}
