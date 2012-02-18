#! /usr/bin/env python
# encoding: utf-8

VERSION = '0.1'
APPNAME = 'c_kurs'

top = '.'

from waflib import Configure, Logs
from subprocess import PIPE, check_call, Popen, CalledProcessError


def options(opt):
    opt.load('compiler_c')
    opt.load('gnu_dirs')

def configure(conf):
    conf.load('compiler_c')
    conf.check_cc(fragment="int main() { return 0; }\n")

    try:
        conf.check_cc(fragment="int main() { return 0; }\n", execute=True)
    except conf.errors.WafError:
        Logs.warn('You are probably using a cross-compiler' +
                  '(disabling specific configuration tests)')
        conf.check_library(test_exec=False)

    conf.check_cc(lib='m', cflags='-Wall',
                  uselib_store='M', mandatory=False)
    conf.check_large_file(mandatory=False)
    conf.check_inline()
    conf.check_endianness()

    conf.multicheck({'header_name':'stdio.h'},
                    {'header_name':'unistd.h'},
                    {'header_name':'stdlib.h'},
                    msg='Checking for standard headers',
                    mandatory=False)

    conf.write_config_header('config.h')


def build(bld):
    bld.recurse('src')


def dist(ctx):
    ctx.algo = 'zip'
    try:
        prg = Popen(['git', 'ls-files'], stdout=PIPE)
        files = prg.communicate()[0]
        ctx.files = ctx.path.ant_glob(files)
    except CalledProcessError:
        ctx.excl = ' **/.waf-1* **/*~ **/*.pyc **/.lock-w* **/build **/.*'
