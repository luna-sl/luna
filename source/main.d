import std.getopt;

import std.stdio : writefln, stderr;
import std.format : format;
import std.logger : Logger, LogLevel;
import std.datetime : Clock;
import std.file : append;
import core.sys.posix.unistd : geteuid;
import std.path : expandTilde;
import std.file : mkdirRecurse;

import liblpkg;
import librepo;

// I would advise keeping this closed.
// If you have naming suggestions, please make an issue.
// TODO: Move to a seperate file.
class WhatDoINameThisLogger : Logger
{
    string filename;
    this(LogLevel lv, string dirname, string filename) @safe
    {
        mkdirRecurse(dirname);
        this.filename = dirname ~ filename;
        super(lv);
    }

    // HACK
    void logToStderrln(string fmt) @trusted
    {
        stderr.writefln(fmt);
    }

    override void writeLogMsg(ref LogEntry payload)
    {
        auto timestamp = payload.timestamp;
        // ref: https://github.com/dlang/phobos/blob/a3f22129dd2a134338ca02b79ff0de242d7f016e/std/logger/core.d#L491
        if (payload.logLevel <= 64)
        {
            writefln(payload.msg);
            append(this.filename, format("%s-%s-%s %s:%s:%s [%s]: %s\n",
                    timestamp.year,
                    timestamp.month,
                    timestamp.day,

                    timestamp.hour,
                    timestamp.minute,
                    timestamp.second,

                    payload.logLevel,

                    payload.msg
            ));
        }
        else
        {
            logToStderrln(format("[%s] %s", payload.logLevel, payload.msg));
            append(this.filename, format("%s-%s-%s %s:%s:%s @ %s:%s():%s [%s]: %s\n",
                    timestamp.year,
                    timestamp.month,
                    timestamp.day,

                    timestamp.hour,
                    timestamp.minute,
                    timestamp.second,

                    payload.file,
                    payload.funcName,
                    payload.line,

                    payload.logLevel,

                    payload.msg
            ));
        }
    }
}

WhatDoINameThisLogger logger;

void main(string[] args)
{
    bool isSu()
    {
        return geteuid() == 0;
    }

    if (isSu)
    {
        logger = new WhatDoINameThisLogger(LogLevel.all, "/var/log/luna/", format("%s.log", Clock.currTime()
                .toUnixTime()));
    }
    else
    {
        logger = new WhatDoINameThisLogger(LogLevel.all, expandTilde("~/.local/state/luna/"), format(
                "%s.log", Clock.currTime()
                .toUnixTime()));
        logger.warning("missing superuser permissions, logging in ~/.local/state/luna instead.");
    }
    logger.info("info");
    logger.error("error");
    locatePackage();
}
