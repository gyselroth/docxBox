[![Build Status](https://travis-ci.org/gyselroth/docxbox.svg?branch=master)](https://travis-ci.org/gyselroth/docxbox)

docxBox
=======

Linux CLI tool for DOCX (OpenXML) analysis and manipulation.


## Table of contents

* [Planned features](#planned-features)
* [Commands](#commands)
  * [List DOCX contents](#list-docx-contents)
    + [Output list of files](#output-list-of-files)
    + [Output list of referenced fonts](#output-list-of-referenced-fonts)
    + [List images information](#list-images-information)
    + [List fields](#list-fields)
    + [List meta data](#list-meta-data)
    + [Reference: Recognized meta attributes](#reference-recognized-meta-attributes)
  * [Output plaintext](#output-plaintext)
      + [Output plaintext segments](#output-plaintext-segments)
  * [Modify document](#modify-document)
      + [Modify meta data](#modify-meta-data)
      + [Replace images](#replace-images)
      + [Replace text](#replace-text)
      + [Randomize document text](#randomize-document-text)
  * [Unzip DOCX: Extract all files, or only media files](#unzip-docx-extract-all-files-or-only-media-files)
  * [Zip files into DOCX](#zip-files-into-docx)  
  * [Output docxBox help or version number](#output-docxbox-help-or-version-number)  
* [Build Instructions](#build-instructions)
* [Running Tests](#running-tests)
* [Changelog](#changelog)
* [Bug Reporting and Feature Requests](#bug-reporting-and-feature-requests)
* [Third Party References](#third-party-references)
* [License](#license)


Planned Features
----------------

* v0.0.2: Generate and insert/replace more complex DOCX markup elements (fields, tables)
* v0.1.0: Batch process sequences of manipulation operations
* v0.1.0: Add optional configuration options via environment vars
* v0.1.0: (Optional) logging of operations
* v1.0.0: Libre-Office compatible appending of two DOCX files into a single one 
  (by XML appending, instead of adding sub-documents) 


Commands
--------

### List DOCX contents

#### Output list of files

Lists files (and directories) contained within a given DOCX, and their attributes:

````docxbox ls foo.docx````

To output as JSON:

````docxbox ls foo.docx --json````  
or ````docxbox ls foo.docx -j````  
or ````docxbox lsj foo.docx````

#### Output list of referenced fonts

````docxbox ls foo.docx --fonts````  
or ````docxbox ls foo.docx -f````  
or ````docxbox lsf foo.docx````  

To output as JSON:

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

To output as JSON:

````docxbox ls foo.docx --images --json````  
````docxbox ls foo.docx -ij````  
or ````docxbox lsi foo.docx --json````  
or ````docxbox lsi foo.docx -j````  
or ````docxbox lsij foo.docx````

#### List fields

````docxbox ls foo.docx --fields````  
or ````docxbox ls foo.docx -d````  
or ````docxbox lsd foo.docx````  

To output as JSON:  

````docxbox ls foo.docx --fields --json````  
or ````docxbox ls foo.docx -dj````  
or ````docxbox lsd foo.docx --json````  
or ````docxbox lsdj foo.docx````  

#### List meta data

docxBox displays only attributes that are contained within the current 
DOCX file (this differs by DOCX version and application), also if given empty.

Output meta data of given DOCX:  

````docxbox ls foo.docx --meta````  
or ````docxbox ls foo.docx -m````  
or ````docxbox lsm foo.docx````

To output as JSON:

````docxbox ls foo.docx --meta --json````  
or ````docxbox ls foo.docx -mj````  
or ````docxbox lsm foo.docx --json````  
or ````docxbox lsm foo.docx -j````  
or ````docxbox lsmj foo.docx````

#### Reference: Recognized meta attributes

* Authors: Creator, lastModifiedBy (``<dc:creator>`` and ``<cp:lastModifiedBy>`` of core.xml)
* Dates: Creation-, modification and print-date  
  (``<dcterms:created>`` and ``<cp:modified>`` and ``<cp:lastPrinted>`` of core.xml) 
* Descriptions: Description, Keywords, Subject, Title   
  (``<dc:description>``, ``<dc:keywords>``, ``<dc:subject>``, ``<dc:title>`` of core.xml)
* Language (``<dc:language>`` of core.xml) 
* Revision (``<cp:revision>`` of core.xml)
* XML schema (````<Properties xmlns ...```` of app.xml)


### Output plaintext

````docxbox txt foo.docx```` outputs the text from document (ATM: w/o header and footer)

#### Output plaintext segments 

````docxbox txt foo.docx --segments````   
or ````docxbox txt foo.docx -s```` 

Outputs the text from document, w/ markup sections separated by newlines.
This can be helpful to identify "segmented" sentences:
Sentences which visually appear as a unit, but are segmented into separate XML 
parent elements for formatting.


### Modify document

#### Modify meta data

DocxBox allows to modify existing attributes, or adds attributes if not present.

* Set creation-date:            ````docxbox mm foo.docx created "2020-01-29T09:21:00Z"````
* Set creator attribute:        ````docxbox mm foo.docx creator "docxBox v0.0.1"````  
* Set description attribute:    ````docxbox mm foo.docx description "Foo bar baz"````  
* Set keywords attribute:       ````docxbox mm foo.docx keywords "Foo bar baz"````  
* Set language attribute:       ````docxbox mm foo.docx language "en-US"````  
* Set lastModifiedBy attribute: ````docxbox mm foo.docx lastModifiedBy "docxBox v0.0.1"````
* Set lastPrinted attribute:    ````docxbox mm foo.docx lastPrinted "2020-01-10T10:31:00Z"````
* Set modification-date:        ````docxbox mm foo.docx modified "2020-01-29T09:21:00Z"````
* Set revision attribute:       ````docxbox mm foo.docx revision 2````
* Set subject attribute:        ````docxbox mm foo.docx subject "Foo bar"````
* Set title attribute:          ````docxbox mm foo.docx title "Foo bar, baz"````


#### Replace images

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg````  
This overwrites the original DOCX with the modified document.

````docxbox rpi foo.docx image1.jpeg /home/replacement.jpeg new.docx````  
This creates a new file: new.docx


#### Replace text

Replace all (case-sensitive) occurrences of given string in DOCX text:

````docxbox rpt foo.docx old new```` updates foo.docx  
````docxbox rpt foo.docx old new new.docx```` creates a new file new.docx  


#### Randomize document text

Replace all text of an existing document by similarly structured random 
"Lorem Ipsum" dummy text, helpful for generating DOCX documents for testing 
purposes:

````docxbox lorem foo.docx```` updates foo.docx  
````docxbox lorem foo.docx new.docx```` creates a new file new.docx  


### Unzip DOCX: Extract all files, or only media files

Unzip all files: ````docxbox uz foo.docx````  

Unzip only media files:  
````docxbox uz foo.docx --media````  
or ````docxbox uz foo.docx -m````  
or ````docxbox uzm foo.docx````


### Zip files into DOCX

````docxbox zip path/to/directory out.docx````


### Output docxBox help or version number

````docxbox````  
or ````docxbox h````    
Outputs docxBox's help text.

````docxbox h <command>```` Outputs more help on a given command.  

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

If you find a bug or have an enhancement request, please file an issue on the 
github repository.


Third Party References
----------------------

Microsoft Office and Word are registered trademarks of Microsoft Corporation.


docxBox was built using the following third party libraries and tools:

| Library                                                         | Description                                                                | License                                                          |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| [nlohmann/json](https://github.com/nlohmann/json)               | JSON for Modern C++                                                        | [MIT License](https://opensource.org/licenses/MIT)               |
| [tfussel/miniz-cpp](https://github.com/tfussell/miniz-cpp)      | Cross-platform header-only C++14 library for reading and writing ZIP files | [MIT License](https://opensource.org/licenses/MIT)               |
| [leethomason/tinyxml2](https://github.com/leethomason/tinyxml2) | A simple, small, efficient, C++ XML parser                                 | [zlib License](https://www.zlib.net/zlib_license.html)           |

| Tool                                                            | Description                                                                | License                                                                                                   |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| [Bats](https://github.com/sstephenson/bats)                     | Bash Automated Testing System                                              | [MIT License](https://opensource.org/licenses/MIT)                                                        |      
| [Clang](https://clang.llvm.org/)                                | A C language family frontend for LLVM                                      | [Apache License](https://www.apache.org/licenses/)                                                        |      
| [Cmake](https://cmake.org/)                                     | Family of tools designed to build, test and package software               | [New BSD License](https://en.wikipedia.org/wiki/New_BSD_License)                                          |      
| [GCC](https://gcc.gnu.org/)                                     | GCC, the GNU Compiler Collection                                           | [GNU General Public License version 3](https://gcc.gnu.org/onlinedocs/libstdc++/manual/appendix_gpl.html) |      
| [Travis CI](https://travis-ci.org)                              | Hosted Continuous Integration Service                                      | [MIT License](https://opensource.org/licenses/MIT)                                                        |

Thanks a lot!


License
-------

docxBox is licensed under [The MIT License (MIT)](https://opensource.org/licenses/MIT)
