#include <config.h>

namespace core {
    Config::Config()
    {
    }

    Config::~Config()
    {
    }

    void Config::Setup(std::string folder) {
        logger.Write(LOG_INFO, "[%s]", __FUNCTION__);

        fpath = folder + "\\" + fname;

        if (!fs::exists(fpath)) {
            logger.Write(LOG_INFO, "[%s] Config not found: %s", __FUNCTION__, fpath.c_str());

            std::string msg = "Config not found:\n" + fpath;

            MessageBox(NULL, msg.c_str(), "WARNING", MB_ICONWARNING);
        }
    }

    void Config::Load() {
        logger.Write(LOG_INFO, "[%s] From %s", __FUNCTION__, fpath.c_str());
        o = json::object();
        std::ifstream _stream(fpath);
        _stream >> o;

        if (!o.contains("Launcher")) {
            logger.Write(LOG_WARN, "[%s] no Launcher", __FUNCTION__);
        }

        auto laucher_o = o["Launcher"];

        logger.Write(LOG_INFO, "[%s] injection_delay", __FUNCTION__);
        if (!laucher_o.contains("injection_delay")) {
            laucher_o["injection_delay"] = injection_delay;
        }
        else {
            injection_delay = laucher_o.at("injection_delay").get<int>();
        }

        logger.Write(LOG_INFO, "[%s] auto_inject", __FUNCTION__);
        if (!laucher_o.contains("auto_inject")) {
            laucher_o["auto_inject"] = auto_inject;
        }
        else {
            auto_inject = laucher_o.at("auto_inject").get<bool>();
        }

        logger.Write(LOG_INFO, "[%s] auto_start", __FUNCTION__);
        if (!laucher_o.contains("auto_start")) {
            laucher_o["auto_start"] = auto_start;
        }
        else {
            auto_start = laucher_o.at("auto_start").get<bool>();
        }

        logger.Write(LOG_INFO, "[%s] is_trial", __FUNCTION__);
        if (!laucher_o.contains("is_trial")) {
            laucher_o["is_trial"] = is_trial;
        }
        else {
            is_trial = laucher_o.at("is_trial").get<bool>();
        }

        LoadFromStrArray(proc_names, "proc_names");
        LoadFromStrArray(window_class_names, "window_class_names");
        LoadFromStrArray(dlls, "dlls");
    }

    void Config::Save() {
        logger.Write(LOG_INFO, "[%s]", __FUNCTION__);

        json::json_pointer p;

        p = json::json_pointer(std::string("/Launcher/injection_delay"));
        o.at(p) = injection_delay;

        p = json::json_pointer(std::string("/Launcher/auto_inject"));
        o.at(p) = auto_inject;

        std::ofstream x(fpath.c_str());

        if (!x) {
            logger.Write(LOG_ERROR, "[%s] Can't write to: %s", __FUNCTION__, fpath.c_str());
            return;
        }

        x << std::setw(4) << o << std::endl;
        x.close();
    }

    void Config::LoadFromStrArray(std::vector<std::string>& to, std::string from) {
        logger.Write(LOG_INFO, "[%s] %s", __FUNCTION__, from.c_str());

        auto laucher_o = o.at("Launcher");

        json array_o;
        bool is_array = false;
        bool contains = laucher_o.contains(from);

        if (contains) {
            array_o = laucher_o[from];
            is_array = array_o.is_array();
        }

        if (contains && is_array) {
            to.clear();
            for (auto it : array_o)
            {
                to.push_back(it);
            }
        }
        else {
            auto arr = json::array();

            for (auto s : to) {
                arr.push_back(s);
            }


            laucher_o[from] = arr;
        }
    }
}


core::Config g_Config;