var assert = require('assert');
var util = require('util');
var console = require('console');
var restify = require('restify');
var fs = require('fs');

// Creates a JSON client
var client = restify.createJsonClient({
  url: 'https://metadata.packet.net/metadata'
});

console.log("mcinit: Starting");

client.get('/metadata', function(err, req, res, obj) {
  assert.ifError(err);

  //console.log(util.inspect(obj));
  process_setup(obj);
});

const mkdirSync = function (dirPath) {
  try {
    fs.mkdirSync(dirPath)
  } catch (err) {
    if (err.code !== 'EEXIST') throw err
  }
}

function write_file(filename,value)
{
   fs.writeFileSync(filename,value);
}
function append_file(filename,value)
{
   fs.appendFileSync(filename,value);
}

     
function add_key(k)
{
    console.log("Adding Key");
    append_file('/root/.ssh/authorized_keys',k + '\n');
}
function process_keys(tk)
{
    mkdirSync('/root/.ssh');
    write_file('/root/.ssh/authorized_keys','');
    tk.forEach(add_key);
}

function process_setup(obj)
{
    write_file('/etc/hostname',obj.hostname + '\n')
    process_keys(obj.ssh_keys);

}

