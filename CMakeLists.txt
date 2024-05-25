idf_component_register(
    SRCS "AutoPID-for-ESP-IDF.cpp"         # list the source files of this component
    INCLUDE_DIRS "./include"        # optional, add here public include directories
    REQUIRES esp_timer         # optional, list the public requirements (component names)
)
