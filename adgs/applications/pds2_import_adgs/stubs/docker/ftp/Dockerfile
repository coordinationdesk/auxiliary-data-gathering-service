FROM alpine:3

RUN apk add vsftpd
ADD vsftpd.conf /etc/vsftpd/vsftpd.conf
RUN adduser -D -G users opemsa
RUN echo 'opemsa:12qwas' | chpasswd -m opemsa

USER opemsa
WORKDIR /home/opemsa
RUN mkdir -p TEST-FTP/emptydir TEST-FTP/tree/dir1 TEST-FTP/tree/dir2

RUN touch TEST-FTP/file-to-delete.txt
RUN dd if=/dev/random of=TEST-FTP/random1 bs=1M count=1
RUN dd if=/dev/zero of=TEST-FTP/tree/dir1/file1-1 bs=1M count=1
RUN dd if=/dev/zero of=TEST-FTP/tree/dir1/file1-2 bs=1M count=1
RUN dd if=/dev/zero of=TEST-FTP/tree/dir2/file2-1 bs=1M count=1
RUN dd if=/dev/zero of=TEST-FTP/tree/dir2/file2-2 bs=1M count=1

USER root

RUN chmod a-w /home/opemsa

EXPOSE 21

ENTRYPOINT [ "vsftpd", "/etc/vsftpd/vsftpd.conf" ]
