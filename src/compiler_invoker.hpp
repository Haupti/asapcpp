#pragma once

#include "config.hpp"
std::string to_build_object_filepath(std::string filename);
std::string compile_file_default(asap_conf *conf, std::string filename,
                                 bool use_ccache, bool silent);
std::string compile_file_production(asap_conf *conf, std::string filename,
                                    bool use_ccache, bool silent);
void link_files_default(asap_conf *conf, std::string object_files_str,
                        std::string output_filename);
void link_files_production(asap_conf *conf, std::string object_files_str,
                           std::string output_filename);
