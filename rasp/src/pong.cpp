#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <regex>
#include "pong.h"
#include "resources.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

pong::pong(ledMatrix* leds): m_leds(leds), m_color_p1(BLUE), m_color_p2(RED)
{
}

void pong::step()
{
    m_leds->clean();
    for(int y = 0; y < NBLINES; y++)
    {
        m_leds->setPixel(5, y, m_color_p1);
        m_leds->setPixel(6, y, m_color_p2);
    }
    for(int y = 6; y < 14; y++)
    {
        m_leds->setPixel(0, y, m_color_p1);
        m_leds->setPixel(LEDPERLINE-1, y, m_color_p2);
    }
    for(int y = 8; y < 12; y++)
    {
        m_leds->setPixel(1, y, m_color_p1);
        m_leds->setPixel(LEDPERLINE-2, y, m_color_p2);
    }
    m_leds->update();

}

std::string pong::get_page_name()
{
    return std::string("/pong.html");
}

void pong::set_parameters(char* json)
{
    rapidjson::Document d;
    d.Parse(json);
    rapidjson::Value& p1 = d["player1"];
    rapidjson::Value& p2 = d["player2"];
    if(p1.IsString() && p1.IsString())
    {
        std::string p1_str = p1.GetString();
        std::string p2_str = p2.GetString();
        std::regex exp ("#([0-9a-fA-f]{6})");
        std::cmatch cm;
        std::cout << "player1" << p1_str << std::endl;
        std::cout << "player2" << p2_str << std::endl;
        std::regex_match(p1_str.c_str(), cm, exp, std::regex_constants::match_default);
        if(cm.size() == 2)
        {
            m_color_p1 =  std::stoi(cm[1], nullptr, 16);
        }
        std::regex_match(p2_str.c_str(), cm, exp, std::regex_constants::match_default);
        if(cm.size() == 2)
        {
            m_color_p2 = std::stoi(cm[1], nullptr, 16);
        }
    }
}

void pong::get_page(std::stringstream& page_stream)
{
    page_stream.write((char*)header_page_html, header_page_html_len);
    page_stream << "        <div id=\"page-wrapper\">" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <h1 class=\"page-header\">Pong</h1>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <form id=\"form\" role=\"form\">" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Joueur 1</span>" << std::endl
                << "                <input type=\"color\" name=\"player1\" class=\"form-control\" value=\"#"<< std::hex << std::setw(6) << std::setfill('0') <<  m_color_p1 << std::dec <<"\">" << std::endl
                << "              </div>" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Joueur 2</span>" << std::endl
                << "                <input type=\"color\" name=\"player2\"class=\"form-control\" value=\"#"<< std::hex << std::setw(6) << std::setfill('0') <<m_color_p2 << std::dec <<"\">" << std::endl
                << "              </div>" << std::endl
                << "              <button id=\"submit\" type=\"submit\" class=\"btn btn-default\">Envoyer</button>" << std::endl
                << "            </form>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "      </div>" << std::endl;
    page_stream.write((char*)footer_page_html, footer_page_html_len);
}


//                 << "              <button type=\"submit\" class=\"btn btn-default\" onclick=\"submitform()\">Envoyer</button>" << std::endl
