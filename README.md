# Easy Travels

Easy Travels is a C++ Desktop Application built using the Qt Framework. It serves as a travel booking and bus ticketing system, featuring role-based access for both Users and Administrators.

## Features

- **User System**: 
  - Register and login securely.
  - Browse available buses and book tickets.
  - View personal booking history.
- **Admin System**: 
  - Separate secure admin login.
  - Add, edit, and manage bus schedules and details.
- **Database Integration**: 
  - Uses SQLite to persistently store user profiles, admin credentials, and bus schedules.
- **Custom UI**: 
  - Designed using Qt Designer with customized stylesheets for a better user experience.

## Technology Stack

- **Language**: C++
- **Framework**: Qt 6 (QtWidgets, QtSql)
- **Database**: SQLite
- **IDE**: Qt Creator

## Prerequisites

To run or build this project locally, you will need:
- [Qt Creator](https://www.qt.io/product/development-tools) and Qt 6.x installed.
- A C++ compiler (like MinGW 64-bit or MSVC, which usually comes with Qt).

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/EasyTravels.git
   ```
2. Open Qt Creator.
3. Select **File > Open File or Project...** and choose the `EasyTravels.pro` file located in the `EasyTravels` folder.
4. Configure the project with your preferred kit (e.g., MinGW 64-bit).
5. Build and Run the project (`Ctrl+R` / `Cmd+R`).

*(Note: The local SQLite database `.db` files are generated automatically or can be provided in the build directory).*

## Future Improvements

While this project demonstrates core desktop application development, some areas planned for future updates include:
- **Security**: Implement secure password hashing (e.g., using bcrypt or similar algorithms) instead of plaintext comparison.
- **Validation**: Enhanced input validation for email and phone numbers.
- **UI Enhancements**: Implementing more dynamic animations and responsive layouts.

## License

This project is open-source and available for educational and portfolio purposes.
