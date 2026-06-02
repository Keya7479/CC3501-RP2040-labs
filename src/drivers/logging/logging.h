#pragma once

/// Represents the priority of a log message. INFORMATION = 0, WARNING = 1, ERROR = 2
enum LogLevel
{
    INFORMATION,
    WARNING,
    ERROR,
};

/**
 * @brief Set the minimum log level to be printed. Correct syntax setLogLevel(LogLevel::level)
 * @param newLevel Messages with a level below this threshold will be discarded
 */
void setLogLevel(LogLevel newLevel);

/**
 * @brief Log a new message with output as [time level]: msg. Correct syntax log(LogLevel::level, msg)
 * @param level The priority and type of log
 * @param msg Message to be printed
 */
void log(LogLevel level, const char *msg);
