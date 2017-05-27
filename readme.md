# mcinit - Micro Cloud Init
Designed to work with Packet Net Metadata Server.
## Description
Does the following:

        1. Read the https://metadata.packet.net/metadata
        2. Updates the hostname provided by packet.net
        3. Create /root/.ssh
        4. Writes the provided keys to /root/.ssh/authorised_hosts

mcnit is packaged as a rhel7 compatible RPM.

## Internals:
   On install via yum/rpm, mcinit is run automatically. 
   httpclient provided by curl lib.
   json provided by jsonc lib.
   rpm packaging provided by togo https://github.com/genereese/togo

## Build Enviroment:
yum install yum install https://github.com/genereese/togo/releases/download/v2.3r7/togo-2.3-7.noarch.rpm
yum install libcurl
yum install libcurl-devel
yum install json-c-devel


