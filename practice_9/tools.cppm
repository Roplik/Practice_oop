module;
import std;

export module tools;






export void clear_screen()
{
    std::cout << "\033[2J\033[H" << std::flush;
}