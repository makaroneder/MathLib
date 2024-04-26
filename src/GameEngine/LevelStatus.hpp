#ifndef LevelStatus_H
#define LevelStatus_H

/// @brief Status of the Level
enum class LevelStatus {
    /// @brief Nothing important is happening
    Normal,
    /// @brief Something failed
    Error,
    /// @brief User lost
    GameOver,
    /// @brief User won
    Victory,
};

#endif