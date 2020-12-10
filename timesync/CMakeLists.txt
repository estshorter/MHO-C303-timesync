add_executable(c303-timesync main.cpp)
find_package(PkgConfig REQUIRED)
pkg_search_module(GATTLIB REQUIRED gattlib)

# message("gattlib: ${GATTLIB}")
# message("gattlib_INCLUDE_DIRS: ${GATTLIB_INCLUDE_DIRS}")
# message("gattlib_LIBRARIES: ${GATTLIB_LIBRARIES}")

include_directories(${GATTLIB_INCLUDE_DIRS}) #インクルードディレクトリ指定 
target_link_libraries(c303-timesync ${GATTLIB_LIBRARIES}) #ライブラリとリンク
target_compile_features(c303-timesync PRIVATE cxx_std_17)
install(TARGETS c303-timesync 
        RUNTIME DESTINATION bin)
install(
    FILES cron-c303-timesync
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ
    WORLD_EXECUTE WORLD_READ 
    DESTINATION /etc/cron.weekly
)
        