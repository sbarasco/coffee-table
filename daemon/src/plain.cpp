#include <regex>
#include <iomanip>
#include "plain.h"
#include "unistd.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define MOTIF_PLAIN        0
#define MOTIF_BORDER       1
#define MOTIF_CHECKERBOARD 2
#define MOTIF_END          3


plain::plain(ledMatrix* leds): m_leds(leds), m_color(WHITE), m_motif(MOTIF_PLAIN)
{
    m_pattern = new uint32_t * [3];
    for(int i = 0; i < MOTIF_END; i++)
    {
        m_pattern[i] = new uint32_t[LEDPERLINE*NBLINES];
        memset(m_pattern[i], 0, LEDPERLINE*NBLINES*sizeof(uint32_t));
    }
    /* Plain pattern */
    for(int i = 0; i < LEDPERLINE*NBLINES; i++)
    {
        m_pattern[MOTIF_PLAIN][i] = WHITE;
    }
    /* Border pattern */
    for(int i = 0; i < LEDPERLINE; i++)
    {
        m_pattern[MOTIF_BORDER][i] = WHITE;
        m_pattern[MOTIF_BORDER][(LEDPERLINE*(NBLINES-1))+i] = WHITE;
    }
    for(int i = 0; i < NBLINES; i++)
    {
        m_pattern[MOTIF_BORDER][i*LEDPERLINE] = WHITE;
        m_pattern[MOTIF_BORDER][i*LEDPERLINE+(LEDPERLINE-1)] = WHITE;
    }
    /* CHECKERBOARD PATTERN */
    for(int i = 0; i < NBLINES; i++)
    {
        for(int j = 0; j < LEDPERLINE; j+=2)
        {
            uint32_t start = i%2;
            m_pattern[MOTIF_CHECKERBOARD][i*(LEDPERLINE)+start+j] = WHITE;
        }
    }
}

plain::~plain()
{
    for(int i = 0; i < MOTIF_END; i++)
    {
        delete [] m_pattern[i];
    }
    delete m_pattern;
}

void plain::step()
{
    m_leds->clean();
    if(m_motif < MOTIF_END)
    {
        for(int i = 0; i < LEDPERLINE; i++)
        {
            for(int j = 0; j < NBLINES; j++)
            {
                uint32_t color = m_pattern[m_motif][i+(j*LEDPERLINE)] & m_color;
                m_leds->setPixel(i, j, color);
            }
        }
    }
    m_leds->update();
}

std::string plain::get_page_name()
{
    return std::string("/plain.html");
}

void plain::set_parameters(char* json)
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
    rapidjson::Value& motif = d["motif"];
    if(motif.IsString())
    {
        std::string motif_str = motif.GetString();
        std::regex exp ("([0-9]+)");
        std::cmatch cm;
        std::regex_match(motif_str.c_str(), cm, exp, std::regex_constants::match_default);
        if(cm.size() == 2)
        {
            m_motif =  std::stoi(cm[1], nullptr, 16);
        }
    }
}

void plain::get_page(std::stringstream& page_stream)
{
    page_stream.write((char*)header_page_html, header_page_html_len);
    page_stream << "        <div id=\"page-wrapper\">" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <h1 class=\"page-header\">Couleur unie</h1>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "        <div class=\"row\">" << std::endl
                << "          <div class=\"col-lg-12\">" << std::endl
                << "            <form id=\"form\" role=\"form\">" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Motif</span>" << std::endl
                << "                <select name=\"motif\" class=\"form-control\">" << std::endl
                << "                  <option value=" << MOTIF_PLAIN;
    if(m_motif == MOTIF_PLAIN)
        page_stream << " selected";
    page_stream << ">Tout</option>" << std::endl
                << "                  <option value=" << MOTIF_BORDER;
    if(m_motif == MOTIF_BORDER)
        page_stream << " selected";
    page_stream << ">Bordure</option>" << std::endl
                << "                  <option value=" << MOTIF_CHECKERBOARD;
    if(m_motif == MOTIF_CHECKERBOARD)
        page_stream << " selected";
    page_stream << ">Damier</option>" << std::endl
                << "                </select>" << std::endl
                << "              </div>" << std::endl
                << "              <div class=\"form-group input-group\">" << std::endl
                << "                <span class=\"input-group-addon\">Couleur</span>" << std::endl
                << "                <input type=\"color\" name=\"color\" class=\"form-control\" value=\"#"
                << std::hex << std::setw(6) << std::setfill('0') <<  m_color << std::dec <<"\">" << std::endl
                << "              </div>" << std::endl
                << "              <button id=\"submit\" type=\"submit\" class=\"btn btn-default\">Envoyer</button>" << std::endl
                << "            </form>" << std::endl
                << "          </div>" << std::endl
                << "        </div>" << std::endl
                << "      </div>" << std::endl;
    page_stream.write((char*)footer_page_html, footer_page_html_len);
}
