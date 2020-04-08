[![Build Status](https://travis-ci.org/gyselroth/docxbox.svg?branch=master)](https://travis-ci.org/gyselroth/docxbox)

docxBox
=======

Linux CLI tool for DOCX (OpenXML) analysis and manipulation.


## Table of contents

* [Features](#features)
  + [Planned features](#planned-features)
* [Commands](#commands)
  * [List files, referenced fonts, images, meta data](#list-files-and-directories-referenced-fonts-images-meta-data)
    + [Output list of files](#output-list-of-files)
    + [Output files list as JSON](#output-files-list-as-json)
    + [Output list of referenced fonts](#output-list-of-referenced-fonts)
    + [Output list of referenced fonts as JSON](#output-list-of-referenced-fonts-as-json)
    + [List images information](#list-images-information)
    + [List images information as JSON](#list-images-information-as-json)
    + [List meta data](#list-meta-data)
    + [List meta data as JSON](#list-meta-data-as-json)
    + [Reference: Recognized meta attributes](#reference-recognized-meta-attributes)
  * [Output plaintext](#output-plaintext)
      + [Output plaintext from DOCX document](#output-plaintext-from-docx-document)
      + [Output plaintext segments from DOCX document](#output-plaintext-segments-from-docx-document)
  * [Modify meta data](#modify-meta-data)
      + [Modify existing (or add) attribute](#modify-existing-or-add-attribute)
  * [Replace images](#replace-images)
    + [Replace image in DOCX](#replace-single-image-in-docx)
    + [Replace image and save to new DOCX](#replace-single-image-and-save-to-new-docx)
  * [Replace text](#replace-text)
    + [Replace all occurrences of string in DOCX text](#replace-all-occurrences-of-string-in-docx-text)
    + [Replace all occurrences of multiple strings in DOCX text](#replace-all-occurrences-of-multiple-strings-in-docx-text)
  * [Unzip DOCX](#unzip-docx)
    + [Unzip all files](#unzip-all-files)
    + [Unzip only media files](#unzip-only-media-files)
  * [Zip files into DOCX](#zip-files-into-docx)  
  * [Output docxBox help](#output-docxbox-help)  
  * [Output docxBox version number](#output-docxbox-version-number)  
* [Build Instructions](#build-instructions)
* [Running Tests](#running-tests)
* [Changelog](#changelog)
* [Bug Reporting and Feature Requests](#bug-reporting-and-feature-requests)
* [Third Party References](#third-party-references)
* [License](#license)


Features
--------

* Output plaintext from DOCX
* List files (and their attributes) in DOCX 
* List fonts (and their metrics) referenced in DOCX 
* List images contained in DOCX 
* List meta data of DOCX
* Modify meta data of DOCX 
* Replace images in DOCX
* Unzip files from DOCX: all files, only media files
* Zip files in given path into new DOCX


### Planned Features

* v0.0.1: Replace occurrences of string(s) in DOCX text
* v0.0.2: Generate and insert/replace more complex DOCX markup elements (merge-fields, tables)
* v0.1.0: Batch process sequences of manipulation operations
* v0.1.0: Add optional configuration options via environment vars
* v0.1.0: (Optional) logging of operations
* v1.0.0: Libre-Office compatible appending of two DOCX files into a single one 
  (by XML appending, instead of adding sub-documents) 


Commands
--------

### List files, referenced fonts, images, meta data

Lists files (and directories) contained within a given DOCX, and their attributes.

#### Output list of files

````docxbox ls foo.docx````

#### Output files list as JSON

````docxbox ls foo.docx --json````  
or ````docxbox ls foo.docx -j````  
or ````docxbox lsj foo.docx````

#### Output list of referenced fonts

````docxbox ls foo.docx --fonts````  
or ````docxbox ls foo.docx -f````  
or ````docxbox lsf foo.docx````  

#### Output list of referenced fonts as JSON

````docxbox ls foo.docx --fonts --json````  
or ````docxbox ls foo.docx -fj````  
or ````docxbox lsf foo.docx --json````  
or ````docxbox lsf foo.docx -j````  
or ````docxbox lsfj foo.docx````

#### List images information

Output list of contained images

````docxbox ls foo.docx --images````  
or ````docxbox ls foo.docx -i````  
or ````docxbox lsi foo.docx````

#### List images information as JSON

````docxbox ls foo.docx --images --json````  
````docxbox ls foo.docx -ij````  
or ````docxbox lsi foo.docx --json````  
or ````docxbox lsi foo.docx -j````  
or ````docxbox lsij foo.docx````

#### List meta data

Output meta data of given DOCX:  

````docxbox ls foo.docx --meta````  
or ````docxbox ls foo.docx -m````  
or ````docxbox lsm foo.docx````

#### List meta data as JSON

````docxbox ls foo.docx --meta --json````  
or ````docxbox ls foo.docx -mj````  
or ````docxbox lsm foo.docx --json````  
or ````docxbox lsm foo.docx -j````  
or ````docxbox lsmj foo.docx````

#### Reference: Recognized meta attributes

* XML schema (````<Properties xmlns ...```` of app.xml) 
* Title (``<dc:title>`` of core.xml)
* Language (``<dc:language>`` of core.xml) 
* Revision (``<cp:revision>`` of core.xml)
* Creator, lastModifiedBy (``<dc:creator>`` and ``<cp:lastModifiedBy>`` of core.xml)
* Creation-, modification and print-date  
  (``<dcterms:created>`` and ``<cp:modified>`` and ``<cp:lastPrinted>`` of core.xml)


### Output plaintext

#### Output plaintext from DOCX document

````docxbox txt foo.docx```` outputs the text from document (ATM: w/o header and footer)

#### Output plaintext segments from DOCX document 

````docxbox txt foo.docx --segments````   
or ````docxbox txt foo.docx -s```` 

Outputs the text from document, w/ markup sections separated by newlines.
This can be helpful to identify "segmented" sentences:
Sentences which visually appear as a unit, but are segmented into separate XML parent elements for formatting.


### Modify meta data

#### Modify existing (or add) attribute

* Set title attribute:          ````docxbox mm foo.docx title "Foo bar, baz"````  
* Set creator attribute:        ````docxbox mm foo.docx creator "docxBox v0.0.1"````  
* Set lastModifiedBy attribute: ````docxbox mm foo.docx lastModifiedBy "docxBox v0.0.1"````
* Set revision attribute:       ````docxbox mm foo.docx revision 2````
* Set lastPrinted attribute:    ````docxbox mm foo.docx lastPrinted "2020-01-10T10:31:00Z"````
* Set language attribute:       ````docxbox mm foo.docx language "en-US"````  
* Set modification-date:        ````docxbox mm foo.docx modified "2020-01-29T09:21:00Z"````
* Set creation-date:            ````docxbox mm foo.docx created "2020-01-29T09:21:00Z"````


### Replace images

#### Replace image in DOCX

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg````  
This overwrites the original DOCX with the modified document.

#### Replace image and save to new DOCX

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg new.docx````


### Replace text

#### Replace all occurrences of string in DOCX text

````docxbox rpt foo.docx old new````

#### Replace all occurrences of multiple strings in DOCX text

Pass search and replacement tuples as escaped JSON

````
docxbox rpt foo.docx "[\"old\":\"new\",\"difficult\":\"easy\",\"dirty\":\"clean\"]"
````


### Unzip DOCX

#### Unzip all files

````docxbox uz foo.docx````

#### Unzip only media files

````docxbox uz foo.docx --media````  
or ````docxbox uz foo.docx -m````  
or ````docxbox uzm foo.docx````


### Zip files into DOCX

````docxbox zip path/to/directory out.docx````


### Output docxBox help

````docxbox````  
or ````docxbox h````    
Outputs docxBox's help text.

````docxbox h <command>```` Outputs more help on a given command.


### Output docxBox version number

````docxbox v```` Outputs the installed docxBox's version number.


Build Instructions
------------------

`cmake CMakeLists.txt; make`


Running tests
-------------

In order to run functional tests, [Bats](https://github.com/sstephenson/bats) must be installed.

Run all tests: `./test.sh`


Changelog
---------

See [Changelog](https://github.com/gyselroth/docxbox/blob/master/CHANGELOG.md)


Bug Reporting and Feature Requests
----------------------------------

If you find a bug or have an enhancement request, please file an issue on the github repository.


Third Party References
----------------------

Microsoft Office and Word are registered trademarks of Microsoft Corporation.


docxBox was built using the following third party libraries and tools:

| Library                                                         | Description                                                                | License                                                          |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [Bats](https://github.com/sstephenson/bats)                     | Bash Automated Testing System                                              | [MIT License](https://opensource.org/licenses/MIT)               |      
| [Clang](https://clang.llvm.org/)                                | A C language family frontend for LLVM                                      | [Apache License](https://www.apache.org/licenses/)               |      
| [Cmake](https://cmake.org/)                                     | Family of tools designed to build, test and package software               | [New BSD License](https://en.wikipedia.org/wiki/New_BSD_License) |      
| [nlohmann/json](https://github.com/nlohmann/json)               | JSON for Modern C++                                                        | [MIT License](https://opensource.org/licenses/MIT)               |
| [tfussel/miniz-cpp](https://github.com/tfussell/miniz-cpp)      | Cross-platform header-only C++14 library for reading and writing ZIP files | [MIT License](https://opensource.org/licenses/MIT)               |
| [leethomason/tinyxml2](https://github.com/leethomason/tinyxml2) | A simple, small, efficient, C++ XML parser                                 | [zlib License](https://www.zlib.net/zlib_license.html)           |
| [Travis CI](https://travis-ci.org)                              | Hosted Continuous Integration Service                                      | [MIT License](https://opensource.org/licenses/MIT)               |

Thanks a lot!


License
-------

docxBox is licensed under [The MIT License (MIT)](https://opensource.org/licenses/MIT)
