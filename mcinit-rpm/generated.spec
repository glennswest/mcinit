# The name of your package
Name: mcinit-rpm

# A short summary of your package
Summary: Micro Cloud Init for Packet Host

# The version of your package
Version: 1.0

# The release number of your package
Release: 10

# Any license you wish to list
License: GNU GPL

# What group this RPM would typically reside in
Group: Applications/System

# Who packaged this RPM
Packager: Glenn West <glennswest@neuralcloudcomputing.com>

# The build architecture of this RPM (noarch/x86_64/i386/etc)
Buildarch: x86_64

# You generally should not need to mess with this setting
Buildroot: %{_tmppath}/%{name}

# Change this extension to change the compression level in your RPM
#  tar / tar.gz / tar.bz2
Source0: %{name}.tar

# If you are having trouble building a package and need to disable
#  automatic dependency/provides checking, uncomment this:
# AutoReqProv: no

# If this package has prerequisites, uncomment this line and
#  list them here - examples are already listed
#Requires: bash, python >= 2.7

# A more verbose description of your package
%description
This is designed to use Packet Host MetaData Service to automatically set hostname and .ssh keys
Source: https://github.com/glennswest/mcinit

# You probably do not need to change this
%define debug_package %{nil}


%prep
%setup -q -c

%build

%install
rsync -a . %{buildroot}/

%clean
rm -rf %{buildroot}

%pre

%post
/usr/local/bin/mcinit > /tmp/mcinit.out

%preun

%postun

#%trigger

#%triggerin

#%triggerun

%changelog
* Sat May 27 2017 Glenn West <glennswest@neuralcloudcomputing.com>
- Initial version.

%files
%attr(0755, root, root) "/usr/local/bin/mcinit"

