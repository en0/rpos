#!/bin/env python


from argparse import ArgumentParser, FileType
from sys import argv, stdout
import re


REGEX="(^\w+|^\w+\s+)=(.*)"


def get_keys(config):
    """Load the key/value pairs from the given file

    Arguments:
        config - the file to read the key/value pairs from

    Returns:
        A dict of the key values found in the config file
    """
    ret = {}
    for line in config:
        m = re.search(REGEX, line)
        if m:
            key, value = m.groups()
            ret[key.strip()] = value.strip()
    return ret


def gen_render(template, keys):
    """Returns a generator that will render each line of the template using the provided key/value pairs

    Arguments:
        template - The template to render
        keys     - The keys to render from

    Returns:
        A Generator that will yield 1 line of the render at a time.
    """
    for line in template:
        for key in keys:
            _k = ''.join(['@', key])
            nline = line.replace(_k, keys[key])
            if nline != line:
                line = nline
                break
        yield line


def render_to_file(config, template, target):
    """Render the template from the keys found in the config file and write to the target

    Arguments:
        config   - The file containing the Key/Value pairs
        template - The template to render
        target   - the output streamm to render to
    """
    keys = get_keys(config)
    for line in gen_render(template, keys):
        target.write(line)


if __name__ == "__main__":
    arg_parser = ArgumentParser(description="Generate configuration file from key words in makefile")
    arg_parser.add_argument("CONFIG", type=FileType('r'))
    arg_parser.add_argument("TEMPLATE", type=FileType('r'))
    arg_parser.add_argument("TARGET", type=FileType('w'), default=stdout, nargs="?")
    args = arg_parser.parse_args(argv[1:])
    render_to_file(args.CONFIG, args.TEMPLATE, args.TARGET)

