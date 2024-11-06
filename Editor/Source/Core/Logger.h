#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace MintEditor
{
	class Logger
	{
	public:
		enum class Level
		{
			Trace,
			Info,
			Warning,
			Error,
			Fatal
		};

		Logger() = default;
		~Logger() = default;

		Logger(const Logger&) = delete; // Prevent copying
		Logger& operator=(const Logger&) = delete; // Prevent assignment
		Logger(Logger&&) = delete; // Prevent moving
		Logger& operator=(Logger&&) = delete; // Prevent move assignment

		static bool Initialize()
		{
			try
			{
				#ifdef _DEBUG
					if (!InitializeDebugConsole()) return false;
				#endif // _DEBUG

				if (!m_Instance) m_Instance = std::make_unique<Logger>();

				const char* appdata = std::getenv("APPDATA");
				if (!appdata) appdata = std::getenv("USERPROFILE");

				std::filesystem::path mintDir = std::filesystem::path(appdata) / "MintEditor";
				std::filesystem::path logsDir = mintDir / "Logs";
				std::filesystem::path backupDir = logsDir / "Backup";
				std::filesystem::path logFile = logsDir / "MintLog.log";

				if (!std::filesystem::exists(mintDir) || !std::filesystem::is_directory(mintDir))
				{
					std::filesystem::create_directory(mintDir);
				}

				if (!std::filesystem::exists(logsDir) || !std::filesystem::is_directory(logsDir))
				{
					std::filesystem::create_directory(logsDir);
				}

				if (std::filesystem::exists(logFile))
				{
					if (!std::filesystem::exists(backupDir) || !std::filesystem::is_directory(backupDir))
					{
						std::filesystem::create_directory(backupDir);
					}

					// File Last Edit Date and Time
					std::filesystem::file_time_type lastEditTime = std::filesystem::last_write_time(logFile);
					time_t lastEditTimeT = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastEditTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()));
					tm* lastEditDate = std::localtime(&lastEditTimeT);

					// Backup File Name
					std::filesystem::path newName = fmt::format("MintLog_{:0>2}-{:0>2}-{}-{:0>2}-{:0>2}-{:0>2}.log",
						lastEditDate->tm_mon + 1, lastEditDate->tm_mday, lastEditDate->tm_year + 1900,
						lastEditDate->tm_hour, lastEditDate->tm_min, lastEditDate->tm_sec);

					std::filesystem::rename(logFile, backupDir / newName);
				}

				auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile.generic_string(), true);
				auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

				fileSink->set_pattern("%^[%H:%M:%S] %n: %v%$");
				consoleSink->set_pattern("%^[%H:%M:%S] %n: %v%$");

				spdlog::sinks_init_list sinks = { fileSink, consoleSink };

				m_Instance->m_Logger = std::make_shared<spdlog::logger>("EditorLogger", sinks); // File logger

				#ifdef _DEBUG
					spdlog::set_level(spdlog::level::trace);
				#else
					spdlog::set_level(spdlog::level::info);
				#endif // _DEBUG

				spdlog::register_logger(m_Instance->m_Logger);

				Logger::Info("Logger initialized");
			}
			catch (const std::exception& e)
			{
				OutputDebugStringA("Logger initialization failed: ");
				OutputDebugStringA(e.what());
				OutputDebugStringA("\n");
				return false;
			}

			return true;
		}

		static bool Shutdown()
		{
			if (!m_Instance || !m_Instance->m_Logger) return false;

			Logger::Info("Logger shutting down");
			m_Instance->m_Logger->flush();
			spdlog::drop_all();
			m_Instance->m_Logger.reset();

			return true;
		}

		static bool InitializeDebugConsole()
		{
			if (!AllocConsole())
			{
				OutputDebugStringA("Logger initialization failed: Failed to open console\n");
				return false;
			}

			FILE* file;
			freopen_s(&file, "CONOUT$", "w", stdout);
			freopen_s(&file, "CONOUT$", "w", stderr);

			SetConsoleTitle("Mint Editor Console");
			
			// Disable Close Button
			HWND consoleWindow = GetConsoleWindow();
			HMENU consoleMenu = GetSystemMenu(consoleWindow, FALSE);
			EnableMenuItem(consoleMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

			return true;
		}

		template<typename... Args>
		static void Trace(const char* fmt, Args&&... args)
		{
			Log(Level::Trace, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Info(const char* fmt, Args&&... args)
		{
			Log(Level::Info, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warning(const char* fmt, Args&&... args)
		{
			Log(Level::Warning, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Error(const char* fmt, Args&&... args)
		{
			Log(Level::Error, fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Fatal(const char* fmt, Args&&... args)
		{
			Log(Level::Fatal, fmt, std::forward<Args>(args)...);
		}

	private:
		template<typename... Args>
		static void Log(Level level, const char* fmt, Args&&... args)
		{
			if (!m_Instance || !m_Instance->m_Logger) return;

			switch (level)
			{
			case Level::Trace:
				m_Instance->m_Logger->trace(fmt, std::forward<Args>(args)...);
				m_Instance->m_Logger->flush();
				break;
			case Level::Info:
				m_Instance->m_Logger->info(fmt, std::forward<Args>(args)...);
				m_Instance->m_Logger->flush();
				break;
			case Level::Warning:
				m_Instance->m_Logger->warn(fmt, std::forward<Args>(args)...);
				m_Instance->m_Logger->flush();
				break;
			case Level::Error:
				m_Instance->m_Logger->error(fmt, std::forward<Args>(args)...);
				m_Instance->m_Logger->flush();
				break;
			case Level::Fatal:
				m_Instance->m_Logger->critical(fmt, std::forward<Args>(args)...);
				m_Instance->m_Logger->flush();
				break;
			}
		}

	private:
		static std::unique_ptr<Logger> m_Instance;
		std::shared_ptr<spdlog::logger> m_Logger;
	};

	inline std::unique_ptr<Logger> Logger::m_Instance = std::make_unique<Logger>();
}