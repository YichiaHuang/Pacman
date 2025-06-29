cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_TowerDefense
    

    # TODO HACKATHON-3 (3/3): Add the new enemy source files here
    GhostSecond/GhostSecond.cpp
    GhostSecond/GhostSecond.hpp
    GhostSecond/Blin.cpp
    GhostSecond/Blin.hpp
    GhostSecond/Cly.cpp
    GhostSecond/Cly.hpp
    GhostSecond/Ink.cpp
    GhostSecond/Ink.hpp
    GhostSecond/Pink.cpp
    GhostSecond/Pink.hpp
    
    Dot/Dot.cpp
    Dot/Dot.hpp
    Dot/NormalDot.cpp
    Dot/NormalDot.hpp
    Dot/Ice.cpp
    Dot/Ice.hpp
    Dot/Speed.cpp
    Dot/Speed.hpp
    Dot/Star.cpp
    Dot/Star.hpp
    Dot/PowerDot.cpp
    Dot/PowerDot.hpp
    Dot/Scroll.cpp
    Dot/Scroll.hpp
    Dot/PD.cpp
    Dot/PD.hpp

    Pacman/Pacman.cpp
    Pacman/Pacman.hpp
    Pacman/Pac.cpp
    Pacman/Pac.hpp

    Slot/SlotMachine.cpp
    Slot/SlotMachine.hpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    
    Ghost/Ghost.cpp
    Ghost/Ghost.hpp
    Ghost/Blinky.cpp
    Ghost/Blinky.hpp
    Ghost/Inky.cpp
    Ghost/Inky.hpp
    Ghost/Pinky.cpp
    Ghost/Pinky.hpp
    Ghost/Clyde.cpp
    Ghost/Clyde.hpp

    # TODO HACKATHON-2 (3/3): Add the new scene source files here
    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.h
    Scene/WinScene.cpp
    Scene/WinScene.hpp
    Scene/SettingsScene.cpp
    Scene/SettingsScene.hpp
    Scene/ScoreboardScene.cpp
    Scene/ScoreboardScene.hpp
    Scene/Win.cpp
    Scene/Win.hpp
    Scene/BoardSecond.cpp
    Scene/BoardSecond.hpp

    Scene/MotionScene.cpp
    Scene/MotionScene.hpp
    Scene/InfoScene.cpp
    Scene/InfoScene.hpp
    Scene/SecondScene.cpp
    Scene/SecondScene.hpp
    Scene/Lose.cpp
    Scene/Lose.hpp
    

    UI/Animation/DirtyEffect.cpp
    UI/Animation/DirtyEffect.hpp
    UI/Animation/ExplosionEffect.cpp
    UI/Animation/ExplosionEffect.hpp
   

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    UI/Component/TextBox.cpp
    UI/Component/TextBox.hpp

    
    
    main.cpp
)

IF (WIN32)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC lib)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()