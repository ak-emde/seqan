#!/usr/bin/env python
"""Execute the tests for the masai program.

The golden test outputs are generated by the script generate_outputs.sh.

You have to give the root paths to the source and the binaries as arguments to
the program.  These are the paths to the directory that contains the 'projects'
directory.

Usage:  run_tests.py SOURCE_ROOT_PATH BINARY_ROOT_PATH
"""
import logging
import os.path
import sys
import glob

# Automagically add util/py_lib to PYTHONPATH environment variable.
path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..',
                                    '..', '..', 'util', 'py_lib'))
sys.path.insert(0, path)

import seqan.app_tests as app_tests

transforms = [
	app_tests.RegexpReplaceTransform("[0-9\.\-e]+ sec", "0.0 sec")
]

defaultFormat = 'raw'
samFormat = 'sam'

def getMapperConf(ph, path_to_program, rl, optionName, optionValue):
    return app_tests.TestConf(
                program=path_to_program,
                redir_stdout=ph.outFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, optionName, optionValue)),
                args=['-'+optionName, str(optionValue),
                      ph.inFile('adeno-genome.fa'),
                      ph.inFile('adeno-reads%d_1.fa' % rl),
                      '-o', ph.outFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, optionName, optionValue, defaultFormat))],
                to_diff=[(ph.inFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, optionName, optionValue, defaultFormat)),
                          ph.outFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, optionName, optionValue, defaultFormat)), 'md5'),
                         (ph.inFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, optionName, optionValue)),
                          ph.outFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, optionName, optionValue)),
                          transforms)])

def main(source_base, binary_base):
    """Main entry point of the script."""

    print 'Executing test for masai'
    print '==========================='
    print
    
    ph = app_tests.TestPathHelper(
        source_base, binary_base,
        'extras/apps/masai/tests')  # tests dir

    # ============================================================
    # Auto-detect the binary path.
    # ============================================================

    path_to_indexer = app_tests.autolocateBinary(
      binary_base, 'bin', 'masai_indexer')

    path_to_mapper = app_tests.autolocateBinary(
      binary_base, 'bin', 'masai_mapper')

    # ============================================================
    # Built TestConf list.
    # ============================================================

    # Build list with TestConf objects, analoguely to how the output
    # was generated in generate_outputs.sh.
    conf_list = []

    # ============================================================
    # Run Indexer Tests
    # ============================================================

    for i in ['sa', 'esa', 'fm', 'qgram']:
        # Get file extensions for the given index type
        exts = [os.path.basename(fname).split('.', 2)[-1]
                for fname in glob.glob(ph.inFile('adeno-index-%s.out.*' % i))]
        conf = app_tests.TestConf(
            program=path_to_indexer,
            redir_stdout=ph.outFile('adeno-index-%s.stdout' % i),
            args=[ph.inFile('adeno-genome.fa'),
            	  '-x', i,
                  '-xp', ph.outFile('adeno-index-%s.out' % i)],
            to_diff=[(ph.inFile('adeno-index-%s.out.%s' % (i,ext)),
                     ph.outFile('adeno-index-%s.out.%s' % (i, ext)), 'md5') for ext in exts] +
                    [(ph.inFile('adeno-index-%s.stdout' % i),
                     ph.outFile('adeno-index-%s.stdout' % i),
                     transforms)])
        conf_list.append(conf)

    # ============================================================
    # Run Mapper Single-End Tests
    # ============================================================

    # We run the following for various read lengths.
    for rl in [100]: #[36, 100]:
    
        # Run with default options.
        conf = app_tests.TestConf(
            program=path_to_mapper,
            redir_stdout=ph.outFile('se-adeno-reads%d_1.stdout' % rl),
            args=[ph.inFile('adeno-genome.fa'),
                  ph.inFile('adeno-reads%d_1.fa' % rl),
                  '-o', ph.outFile('se-adeno-reads%d_1.out.%s' % (rl, defaultFormat))],
            to_diff=[(ph.inFile('se-adeno-reads%d_1.out.%s' % (rl, defaultFormat)),
                      ph.outFile('se-adeno-reads%d_1.out.%s' % (rl, defaultFormat)),
                      'md5'),
                     (ph.inFile('se-adeno-reads%d_1.stdout' % rl),
                      ph.outFile('se-adeno-reads%d_1.stdout' % rl),
                      transforms)])
        conf_list.append(conf)

        # Run with different seed length.
        for optionValue in [16, 50]:
            optionName='sl'
            conf_list.append(getMapperConf(ph, path_to_mapper, rl, optionName, optionValue))

        # Run with different indices.
#        for optionValue in ['esa', 'fm', 'qgram']:
#            optionName='i'
#            conf_list.append(getMapperConf(ph, path_to_mapper, rl, optionName, optionValue))

        # Run with different mapping modes.
        for optionValue in ['all']:
            optionName='mm'
            conf_list.append(getMapperConf(ph, path_to_mapper, rl, optionName, optionValue))

        # Run with different absolute number of errors.
        for optionValue in [0, 1, 2, 3, 4]:
            optionName='e'
            conf_list.append(getMapperConf(ph, path_to_mapper, rl, optionName, optionValue))

        # Run with sam output format.
        conf = app_tests.TestConf(
            program=path_to_mapper,
            redir_stdout=ph.outFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, 'of', samFormat)),
            args=[ph.inFile('adeno-genome.fa'),
                  ph.inFile('adeno-reads%d_1.fa' % rl),
                  '-o', ph.outFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, 'of', samFormat, samFormat))],
            to_diff=[(ph.inFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, 'of', samFormat, samFormat)),
                      ph.outFile('se-adeno-reads%d_1-%s%s.out.%s' % (rl, 'of', samFormat, samFormat))),
                     (ph.inFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, 'of', samFormat)),
                      ph.outFile('se-adeno-reads%d_1-%s%s.stdout' % (rl, 'of', samFormat)),
                      transforms)])
        conf_list.append(conf)
        
        # Run without gaps.
        conf = app_tests.TestConf(
            program=path_to_mapper,
            redir_stdout=ph.outFile('se-adeno-reads%d_1-nogaps.stdout' % rl),
            args=[ph.inFile('adeno-genome.fa'),
                  ph.inFile('adeno-reads%d_1.fa' % rl),
                  '--no-gaps',
                  '-o', ph.outFile('se-adeno-reads%d_1-nogaps.out.%s' % (rl, defaultFormat))],
            to_diff=[(ph.inFile('se-adeno-reads%d_1-nogaps.out.%s' % (rl, defaultFormat)),
                      ph.outFile('se-adeno-reads%d_1-nogaps.out.%s' % (rl, defaultFormat)),
                      'md5'),
                     (ph.inFile('se-adeno-reads%d_1-nogaps.stdout' % rl),
                      ph.outFile('se-adeno-reads%d_1-nogaps.stdout' % rl),
                      transforms)])
        conf_list.append(conf)

    # ============================================================
    # Run Mapper Paired-End Tests
    # ============================================================

    # TODO(esiragusa): Write Paired-End tests.

    # Execute the tests.
    failures = 0
    for conf in conf_list:
        res = app_tests.runTest(conf)
        # Output to the user.
        print ' '.join([conf.program] + conf.args),
        if res:
             print 'OK'
        else:
            failures += 1
            print 'FAILED'

    # Cleanup.
    ph.deleteTempDir()

    print '=============================='
    print '     total tests: %d' % len(conf_list)
    print '    failed tests: %d' % failures
    print 'successful tests: %d' % (len(conf_list) - failures)
    print '=============================='
    # Compute and return return code.
    return failures != 0


if __name__ == '__main__':
    sys.exit(app_tests.main(main))
