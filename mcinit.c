#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string.h>
#include <json/json.h>
#include <libgen.h>
#include <time.h>

struct string {
    char *ptr;
    size_t len;
};

void append_file(char *thepath, char *thetext);

#include <libgen.h>

char *current_time()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  return ( asctime (timeinfo) );
}

void logit(char *thetext)
{
     append_file("/tmp/mcinit.out",thetext);
}

void mkdir_recursive(char *path)
{
    char *subpath, *fullpath, *thepath;

    fullpath = strdup(path);
    subpath = strdup(path);
    dirname(subpath);
    if (strlen(subpath) > 1)
        mkdir_recursive(subpath);
    free(subpath);
    mkdir(fullpath);
    free(fullpath);
}

void init_string(struct string *s){
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL){
       fprintf(stderr, "malloc() failed\n");
       exit(EXIT_FAILURE);
       };
    s->ptr[0] = '\0';
}
       
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

char *trim(char *thestring)
{
char *ptr;
char *result;

        ptr = thestring;
        result = thestring;
	while(*ptr != '\0'){
           if (*ptr == '"'){
              ptr++;
             } else {
               *result++ = *ptr++;
             }
           }
        *result = '\0';
        return(result);
}

void truncate_file(char *thepath)
{
FILE *f;
	f = fopen(thepath,"w");
        if (f == NULL){
           printf("Cannot truncate %s\n",thepath);
           return;
           }


}

void write_file(char *thepath,char *thestring)
{
FILE *f;
	f = fopen(thepath,"w");
        if (f == NULL){
           printf("Cannot write to %s - Value %s\n",thepath,thestring);
           return;
           }
        fprintf(f, "%s\n", thestring);
        fclose(f);
}

void append_file(char *thepath,char *thestring)
{
FILE *f;
        f = fopen(thepath,"a+");
        if (f == NULL){
           printf("Cannot append to %s - Value %s\n",thepath,thestring);
           return;
           }
        fprintf(f, "%s", thestring);
        fclose(f);
}

void set_hostname(json_object * jobj)
{
char *hostname;
struct json_object *obj;
char buf[256];

    json_object_object_get_ex (jobj,"hostname",&obj);
    hostname = (char *)json_object_to_json_string(obj);
    trim(hostname);
    write_file("/etc/hostname",hostname);
    sprintf(buf,"Setting hostname to %s\n",hostname);
    logit(buf);
}

void set_ssh_keys(json_object * jobj)
{
struct json_object *obj, *okey;
char *key;
int i;

    json_object_object_get_ex (jobj,"ssh_keys",&obj);
    mkdir_recursive("/root/.ssh");
    truncate_file("/root/.ssh/authorized_keys");
    for(i = 0; i < json_object_array_length(obj); i++) {
       okey = json_object_array_get_idx(obj, i);
       key = (char *)json_object_to_json_string(okey);
       trim(key);
       strcat(key,"\n");
       append_file("/root/.ssh/authorized_keys",key);
       }
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  struct string s;
  char status[256];

  curl = curl_easy_init();
  if(curl) {
    init_string(&s);
    curl_easy_setopt(curl, CURLOPT_URL, "https://metadata.packet.net/metadata");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "metadata.packet.net failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
  sprintf(status,"mcinit: Updated System Settings at %s",current_time());
  truncate_file("/tmp/mcinit.out");
  logit(status);
  json_object *jobj = json_tokener_parse(s.ptr);
  set_hostname(jobj);
  set_ssh_keys(jobj);
  return 0;
}

