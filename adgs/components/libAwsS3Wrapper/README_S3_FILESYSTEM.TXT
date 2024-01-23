
This technical note deals with the filesystem in S3.

PARAGRAPH 1: creating "fake" filesystems in S3.

- First, the set of different buckets you can access on your cloud account is _not_ a filesystem: there is an API to list the buckets, and specific API to check wether a bucket exists.
- All the API expects a _couple_ of entries, like: { buket_name, object_name }
For example: get_metadata ( "my_bucket_22", "dir1/dir5/file" )
- So, a bucket is _not_ the upper level in your directory tree -- it's more like the "mount point" or "coordinates for the root directory"
- Given a bucket, the "object name" is more similar to the usual "PATH" concept:
valid object names are: "file22.txt"   "longfile.a.b.c"  "dir1/dir2/file22"

It depends on the S3 implementation, but usually inside the buckets THERE ISN'T any directory structure !
That is: a bucket is flat. A bucket just contains a plain list of files (all the entries are separate files, just files).

But ...
- You are allowed to use "/" inside the name of a file.
- file1, "dir2/file5" and "dir2/dir3/file22" are three valid valid names, all flattened in your bucket.
- Conventionally (but it's just a SOFTWARE CONVENTION), client allows the user to navigate a virtual level of directories.
How ?
- If you create the valid file: "dir2/dir3/file22",
the client will read the name and split the "/"s in order to show you a directory "dir2" containing another directory "dir3" which contains "file22"
- These directory are totally non-existing. The bucket contains ONE SINGLE ENTRY that is "dir2/dir3/file22" 
Nevertheless, if you create these three entries:
"dir2/dir3/file22" 
"dir2/dir3/file23" 
"dir2/dir3/file24" 

as you might expect in any standard filesystem, the client will show a tree of directories, where directory dir3, inside dir2, contains 3 files file22,file23,file24.

The only problem is the following:
- Assume you create the file: "dir2/dir3/file22"
- The client will show you the directory tree: dir2/dir3 containing file22.
- If you remove the file "dir2/dir3/file22", of course also the tree will immediately disappear !
- So the removal of the LAST file in a (virtual) tree of directories will also remove the tree itself. This is not expected in the filesystem-world.
- To solve this problem, if you WANT to offer a sort of "virtual-filesystem" you should also create the "directory entries".
- By convention, you create an empty file with a name terminating with "/". THE "/" SUFFIX IS A "DIRECTORY MARKER"
- So, assume that you create all these FILES:
dir2/   [empty file]
dir2/dir3/  [empty file]
dir2/dir3/file22  [data file]

in this case, the client will show you the usual "virtual" filesystem (a tree of directories), but, if you remove "dir2/dir3/file22" the "tree" will remain (the client still shows you two directories: dir2 containing dir3, and dir3 empty.

Accordingly to this, the clients have usually these functions:
- CREATE DIRECTORY (name)
which just creates an empty file with name: "name/" [followed by slash]
- REMOVE DIRECTORY (name)
which just removes a file with name: "name/" [followed by slash]
- LIST DIRECTORY (name)
which justs scan all the entries, showing you the ones beginning with "name/" (e.g. name/file1, name/dir2/file44) where name could be dir33/dir55
- DOES DIRECTORY EXIST (name)
which check if then entry "name/" is a valid file -- that is a virtual directory entry.

and so on ...
Note that, by convention, the directory DOES NOT start with "/". They finish with "/".
So "/dir1" "/dir1/" are INVALID names. 
"dir1/" is a valid name. "dir" is an INVALID name -- it must terminate with slash.

Following the (optional) conventions above, any S3 flat-bucket-listing in indistinguishable by a standard FTP-like filesystem.


PARAGRAPH 2: Buckets and object names

The bucket name is outside the system of object names in S3.
Indeed, all the API expects a pair of enties (buket_name, object_name)
so you are expected to keep two separate informations (in a simple pair of strings).

Problem:
Your S3 client is embedded in a generic framework. This framework allow you to store/retrieve the position of files (filesystem-like, URL-like).
In this case, you don't have enough "room" to store the pair of information (bucket; path).

A relative standard way to solve this issue is the following.
You introduce the concepts of absolute path and relative path:
- An absolute path starts with the name of the bucket
- A relative path DOES NOT contain the name of the bucket.

So, if might create an absolute path from ("bucket4" ; "dir2/file5")
by writing "/bucket4/dir2/file".
And viceversa, you can easily split and "absolute path" by stripping the first component of the path, and call it bucket.

NOTE THAT THIS CONCEPT DOES NOT EXIST IN S3 AND IN MOST CLIENTS.

In out client we DO NOT USE the absolute paths concept.
To avoid confusion, and to use the same conventions as the S3 API, we always use the set (buket_name, object_name) to identify the position of any file in S3.

Anyway, a couple of very simple methods is provided:
absolute_to_relative (string -> pair of strings)
relative_to_absolute (pair of strings -> string)

to deal with a non-s3-aware infrastructure.

Assume that you use a not-S3-aware infrastructure to store/retrieve the file paths:
Example 1, read a file:
   abs_path = get_path_from_db()  -->  "/bucket2/dir3/file2"
   ...
   ... before accessing to S3 API:
   (bucket; rel_path) = relative_to_absolute (abs_path)
-> bucket = "bucket2", rel_path = "dir3/file2"
   read_file_from_S3 (bucket, rel_path, local_file_to_save)


Example 2, write a file on S3 and store the file name to the infrastructure:
    bucket = "bucket3" ; rel_path = "dir5/file8" 
    store_file_to_S3 (local_file_to_read, bucket, rel_path)
    ...
    ... in order to save the file name in a non-S3-aware infrastructure:
    abs_path = relative_to_absolute(bucket, rel_path)
--> abs_path = "bucket3/dir5/file8"
    store_filename_to_db(unique_key, date, md5, abs_path)


- Remember to STRIP AWAY all the "/".
E.G. given the absolute path: "/bucket5/dir2/file3.txt"
The extracted name of bucket must be "bucket5" (not: /bucket5, bucket5/ ...)
The extracted path must be: "dir2/file3.txt" (not: /dir2...)

- In theory, you should assume that:
"starting with / is an absolute path"  "not starting with / is a relative path"
so:
"/bucket5/file5" is the pair (bucket5, file5)
while "dir1/dir2/file3" is the s3 object "dir1/dir2/file3" in some bucket specified elsewhere.

But it's better not mix the two concepts, and use a pair of strings in all the s3-aware code and the absolute path only when reading/storing this information elsewhere.


