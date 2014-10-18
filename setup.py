# -*- coding: utf-8 -*-
# $Id$

from setuptools import setup, find_packages, Extension
import sys, os

version = "0.1"

combinator = Extension("combinator", sources=["llucia/c/combinatormodule.c"])

setup(name="llucia",
      version=version,
      description="Distribuir calculs de TJ entre ordinadors",
      long_description="",
      classifiers=[], # Get strings from http://pypi.python.org/pypi?%3Aaction=list_classifiers
      keywords="",
      author="Llucia Mauri i Alexis Roda",
      author_email="alexis.roda.villalonga@gmail.com",
      url="",
      license="GPL",
      packages=find_packages(exclude=["ez_setup", "examples", "tests"]),
      include_package_data=True,
      zip_safe=False,
      install_requires=[
          # -*- Extra requirements: -*-
      ],
      entry_points="""
      # -*- Entry points: -*-
      """,
      ext_modules = [combinator],
      )
