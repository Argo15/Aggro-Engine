QT_BUILD_PARTS += libs tools
QT_QCONFIG_PATH = 

host_build {
    QT_CPU_FEATURES.x86_64 =  sse sse2
} else {
    QT_CPU_FEATURES.x86_64 =  sse sse2
}
QT_COORD_TYPE += double
CONFIG += precompile_header pcre release compile_examples sse2 sse3 ssse3 sse4_1 sse4_2 avx avx2 largefile
QMAKE_QT_VERSION_OVERRIDE = 5
LIBS           += -LC:/utils/postgresql/pgsql/lib -LC:/utils/my_sql/my_sql/lib
INCLUDEPATH    += C:/utils/postgresql/pgsql/include C:/utils/my_sql/my_sql/include
sql-plugins    += mysql odbc psql sqlite
styles         += windows fusion windowsxp windowsvista
