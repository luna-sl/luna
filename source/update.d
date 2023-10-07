module update;

import std.stdio : writefln;
import std.file : readText, exists, mkdirRecurse;
import std.string : strip, split;
import std.path : baseName;
import std.net.curl : download;
import std.format : format;

import main;

void updateRepos(string[] args) {
    if (!exists("/etc/luna/repos.conf")) {
        logger.fatal("cannot access '/etc/luna/repos.conf': no such file or directory");
    }
    if(!exists("/var/lib/luna/repos.conf.d/")){
        mkdirRecurse("/var/lib/luna/repos.conf.d/");
    }
    logger.info("updating repos...");
    string[] repos = split(strip(readText("/etc/luna/repos.conf")), "\n");
    for(int i = 0; i < repos.length; ++i){
        string fname = baseName(repos[i]);
        logger.info(format("updating %s (%s/%s)", fname, i+1, repos.length));
        download(repos[i], "/var/lib/luna/repos.conf.d/" ~ fname);
    }
    return;
}