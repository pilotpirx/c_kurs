#! /usr/bin/env python
# encoding: utf-8

VERSION = '0.1'
APPNAME = 'c_kurs'

top = '.'

from waflib import Configure, Logs
from subprocess import PIPE, check_call, Popen, CalledProcessError


def options(opt):
    opt.load('compiler_c')
    opt.load('waf_unit_test')
    opt.add_option('--debug', action='store_true',
                   default=False, dest='debug',
                   help='Debug mode')

    opt.add_option('--debug',
                   action='store_true',
                   default=False,
                   help='Build debug variant',
                   dest='debug')

    opt.add_option('--profile',
                   action='store_true',
                   default=False,
                   help='Enable profiling',
                   dest='profile')


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
                  uselib_store='M', mandatory=True)

    conf.check_cc(lib='check', cflags='-Wall',
                  uselib_store='check', mandatory=False,
                  errmsg="could not find check. Unit tests disabled")

    conf.multicheck({'header_name':'stdio.h'},
                    {'header_name':'unistd.h'},
                    {'header_name':'stdlib.h'},
                    msg='Checking for standard headers',
                    mandatory=True)

    conf.load('waf_unit_test')

    if conf.options.debug:
        debug_compile_flags = ['-g', '-O0', '-Wall', '-Wextra']
        conf.env.append_value('CFLAGS', debug_compile_flags)


    if conf.options.profile:
        conf.env.append_value('CPPFLAGS', '-pg')
        conf.env.append_value('LINKFLAGS', '-pg')



def build(bld):
    bld.recurse('src tests')
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)


def dist(ctx):
    ctx.algo = 'zip'
    try:
        prg = Popen(['git', 'ls-files'], stdout=PIPE)
        files = prg.communicate()[0]
        ctx.files = ctx.path.ant_glob(files)
    except CalledProcessError:
        ctx.excl = ' **/.waf-1* **/*~ **/*.pyc **/.lock-w* **/build **/.*'
