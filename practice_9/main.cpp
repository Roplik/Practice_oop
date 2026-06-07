#include <sqlite3.h>
#include <fmt/core.h>
#include <fmt/color.h>
import std;
import tools;


void Debug(const int& what)
{
    fmt::print(fg(fmt::color::red), "Было введено: {}\n", what);
}

void add(sqlite3* db)
{
    fmt::print(fg(fmt::color::green) | bg(fmt::color::white), "========= ДОБАВЛЕНИЕ ЗАДАЧИ =========");





    return;
}


unsigned short int menu()
{   
    unsigned short int res = 0;
    //clear_screen();
    fmt::println("Меню:");
    fmt::print("\n");
    fmt::println("('1') Добавить");
    fmt::println("('2') Прочитать");
    fmt::println("('3') Изменить");
    fmt::println("('4') Выйти");
    fmt::print("\nВведите команду: ");
    std::cin >> res;
    Debug(res);
    return res;
}

void Commands(sqlite3* db, bool& status, const int& cmd)
{
    switch (cmd)
    {
    case 1:
        //add(db);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        status = false;
        break;
    
    default:
        fmt::print(fg(fmt::color::red), "НЕПРАВИЛЬНАЯ КОММАНДА\n");
        break;
    }
    return;
}




int main()
{   
    sqlite3* db = nullptr;
    bool is_running = true;
    unsigned short int command = 0;


    int file = sqlite3_open("todo", &db);
    if (file != SQLITE_OK)
    {
        std::cerr << "Ошибка: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

     const char* sql = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            title TEXT NOT NULL,
            description TEXT,
            priority INTEGER DEFAULT 2,
            completed INTEGER DEFAULT 0,
            due_date DATE,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";



    while (is_running)
    {
        command = menu();
        Commands(db, is_running, command);
    }

    sqlite3_close(db);
    return 0;
    
}