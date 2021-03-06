import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        Depends { name: "Qt.core" }
        Depends { name: "Qt.network" }
        Depends { name: "Qt.sql" }

        cpp.cxxLanguageVersion: "c++11"

        cpp.defines: [
            // The following define makes your compiler emit warnings if you use
            // any feature of Qt which as been marked deprecated (the exact warnings
            // depend on your compiler). Please consult the documentation of the
            // deprecated API in order to know how to port your code away from it.
            "QT_DEPRECATED_WARNINGS",

            // You can also make your code fail to compile if you use deprecated APIs.
            // In order to do so, uncomment the following line.
            // You can also select to disable deprecated APIs only up to a certain version of Qt.
            //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
        ]

        Properties {
            condition: qbs.buildVariant === "debug"
            cpp.defines: outer.concat("NEVERRUN_PW_DEVELOPMENT")
        }

        consoleApplication: true
        files: [
            "datagrams/bnds.cpp",
            "datagrams/bnds.hpp",
            "datagrams/bnes.cpp",
            "datagrams/bnes.hpp",
            "datagrams/bnxi.cpp",
            "datagrams/bnxi.hpp",
            "main.cpp",
            "neverrunpwudpserice.cpp",
            "neverrunpwudpserice.h",
            "nwdatagram.cpp",
            "nwdatagram.hpp",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
