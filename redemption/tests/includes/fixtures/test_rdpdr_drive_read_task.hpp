const char outdata[] = /* NOLINT */
{
// RdpdrDriveReadTask::run |
// DeviceIOResponse: DeviceId=0 CompletionId=8 IoStatus=0x00000000 |
// RdpdrDriveReadTask::run: Length=2048 |
// RdpdrDriveReadTask::run: NumberOfBytesRead=1580 |
// Sending on target (-1) 8 bytes |
/* 0000 */ "\x14\x08\x00\x00\x01\x00\x00\x00"                                 //........ |
// Sent dumped on target (-1) 8 bytes |
// Sending on target (-1) 1600 bytes |
/* 0000 */ "\x72\x44\x43\x49\x00\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00" //rDCI............ |
/* 0010 */ "\x00\x08\x00\x00\x6f\x66\x20\x64\x61\x74\x61\x20\x62\x79\x74\x65" //....of data byte |
/* 0020 */ "\x73\x2c\x0a\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" //s,..             |
/* 0030 */ "\x72\x65\x63\x6f\x72\x64\x2d\x73\x74\x72\x75\x63\x74\x75\x72\x65" //record-structure |
/* 0040 */ "\x2c\x20\x20\x20\x77\x68\x65\x72\x65\x20\x74\x68\x65\x20\x66\x69" //,   where the fi |
/* 0050 */ "\x6c\x65\x20\x69\x73\x20\x6d\x61\x64\x65\x20\x75\x70\x20\x6f\x66" //le is made up of |
/* 0060 */ "\x20\x73\x65\x71\x75\x65\x6e\x74\x69\x61\x6c\x0a\x20\x20\x20\x20" // sequential.     |
/* 0070 */ "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" //                 |
/* 0080 */ "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x72\x65\x63\x6f" //            reco |
/* 0090 */ "\x72\x64\x73\x2c\x0a\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" //rds,..           |
/* 00a0 */ "\x20\x20\x61\x6e\x64\x20\x70\x61\x67\x65\x2d\x73\x74\x72\x75\x63" //  and page-struc |
/* 00b0 */ "\x74\x75\x72\x65\x2c\x20\x77\x68\x65\x72\x65\x20\x74\x68\x65\x20" //ture, where the  |
/* 00c0 */ "\x66\x69\x6c\x65\x20\x69\x73\x20\x6d\x61\x64\x65\x20\x75\x70\x20" //file is made up  |
/* 00d0 */ "\x6f\x66\x20\x69\x6e\x64\x65\x70\x65\x6e\x64\x65\x6e\x74\x0a\x20" //of independent.  |
/* 00e0 */ "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" //                 |
/* 00f0 */ "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x69" //               i |
/* 0100 */ "\x6e\x64\x65\x78\x65\x64\x20\x70\x61\x67\x65\x73\x2e\x0a\x0a\x20" //ndexed pages...  |
/* 0110 */ "\x20\x20\x20\x20\x20\x20\x20\x20\x46\x69\x6c\x65\x2d\x73\x74\x72" //        File-str |
/* 0120 */ "\x75\x63\x74\x75\x72\x65\x20\x69\x73\x20\x74\x68\x65\x20\x64\x65" //ucture is the de |
/* 0130 */ "\x66\x61\x75\x6c\x74\x20\x74\x6f\x20\x62\x65\x20\x61\x73\x73\x75" //fault to be assu |
/* 0140 */ "\x6d\x65\x64\x20\x69\x66\x20\x74\x68\x65\x20\x53\x54\x52\x55\x63" //med if the STRUc |
/* 0150 */ "\x74\x75\x72\x65\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x63\x6f" //ture.         co |
/* 0160 */ "\x6d\x6d\x61\x6e\x64\x20\x68\x61\x73\x20\x6e\x6f\x74\x20\x62\x65" //mmand has not be |
/* 0170 */ "\x65\x6e\x20\x75\x73\x65\x64\x20\x62\x75\x74\x20\x62\x6f\x74\x68" //en used but both |
/* 0180 */ "\x20\x66\x69\x6c\x65\x20\x61\x6e\x64\x20\x72\x65\x63\x6f\x72\x64" // file and record |
/* 0190 */ "\x20\x73\x74\x72\x75\x63\x74\x75\x72\x65\x73\x0a\x20\x20\x20\x20" // structures.     |
/* 01a0 */ "\x20\x20\x20\x20\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x61\x63\x63" //     must be acc |
/* 01b0 */ "\x65\x70\x74\x65\x64\x20\x66\x6f\x72\x20\x22\x74\x65\x78\x74\x22" //epted for "text" |
/* 01c0 */ "\x20\x66\x69\x6c\x65\x73\x20\x28\x69\x2e\x65\x2e\x2c\x20\x66\x69" // files (i.e., fi |
/* 01d0 */ "\x6c\x65\x73\x20\x77\x69\x74\x68\x20\x54\x59\x50\x45\x20\x41\x53" //les with TYPE AS |
/* 01e0 */ "\x43\x49\x49\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x6f\x72\x20" //CII.         or  |
/* 01f0 */ "\x45\x42\x43\x44\x49\x43\x29\x20\x62\x79\x20\x61\x6c\x6c\x20\x46" //EBCDIC) by all F |
/* 0200 */ "\x54\x50\x20\x69\x6d\x70\x6c\x65\x6d\x65\x6e\x74\x61\x74\x69\x6f" //TP implementatio |
/* 0210 */ "\x6e\x73\x2e\x20\x20\x54\x68\x65\x20\x73\x74\x72\x75\x63\x74\x75" //ns.  The structu |
/* 0220 */ "\x72\x65\x20\x6f\x66\x20\x61\x20\x66\x69\x6c\x65\x0a\x20\x20\x20" //re of a file.    |
/* 0230 */ "\x20\x20\x20\x20\x20\x20\x77\x69\x6c\x6c\x20\x61\x66\x66\x65\x63" //      will affec |
/* 0240 */ "\x74\x20\x62\x6f\x74\x68\x20\x74\x68\x65\x20\x74\x72\x61\x6e\x73" //t both the trans |
/* 0250 */ "\x66\x65\x72\x20\x6d\x6f\x64\x65\x20\x6f\x66\x20\x61\x20\x66\x69" //fer mode of a fi |
/* 0260 */ "\x6c\x65\x20\x28\x73\x65\x65\x20\x74\x68\x65\x20\x53\x65\x63\x74" //le (see the Sect |
/* 0270 */ "\x69\x6f\x6e\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x6f\x6e\x20" //ion.         on  |
/* 0280 */ "\x54\x72\x61\x6e\x73\x6d\x69\x73\x73\x69\x6f\x6e\x20\x4d\x6f\x64" //Transmission Mod |
/* 0290 */ "\x65\x73\x29\x20\x61\x6e\x64\x20\x74\x68\x65\x20\x69\x6e\x74\x65" //es) and the inte |
/* 02a0 */ "\x72\x70\x72\x65\x74\x61\x74\x69\x6f\x6e\x20\x61\x6e\x64\x20\x73" //rpretation and s |
/* 02b0 */ "\x74\x6f\x72\x61\x67\x65\x20\x6f\x66\x0a\x20\x20\x20\x20\x20\x20" //torage of.       |
/* 02c0 */ "\x20\x20\x20\x74\x68\x65\x20\x66\x69\x6c\x65\x2e\x0a\x0a\x20\x20" //   the file...   |
/* 02d0 */ "\x20\x20\x20\x20\x20\x20\x20\x54\x68\x65\x20\x22\x6e\x61\x74\x75" //       The "natu |
/* 02e0 */ "\x72\x61\x6c\x22\x20\x73\x74\x72\x75\x63\x74\x75\x72\x65\x20\x6f" //ral" structure o |
/* 02f0 */ "\x66\x20\x61\x20\x66\x69\x6c\x65\x20\x77\x69\x6c\x6c\x20\x64\x65" //f a file will de |
/* 0300 */ "\x70\x65\x6e\x64\x20\x6f\x6e\x20\x77\x68\x69\x63\x68\x20\x68\x6f" //pend on which ho |
/* 0310 */ "\x73\x74\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x73\x74\x6f\x72" //st.         stor |
/* 0320 */ "\x65\x73\x20\x74\x68\x65\x20\x66\x69\x6c\x65\x2e\x20\x20\x41\x20" //es the file.  A  |
/* 0330 */ "\x73\x6f\x75\x72\x63\x65\x2d\x63\x6f\x64\x65\x20\x66\x69\x6c\x65" //source-code file |
/* 0340 */ "\x20\x77\x69\x6c\x6c\x20\x75\x73\x75\x61\x6c\x6c\x79\x20\x62\x65" // will usually be |
/* 0350 */ "\x20\x73\x74\x6f\x72\x65\x64\x20\x6f\x6e\x0a\x20\x20\x20\x20\x20" // stored on.      |
/* 0360 */ "\x20\x20\x20\x20\x61\x6e\x20\x49\x42\x4d\x20\x4d\x61\x69\x6e\x66" //    an IBM Mainf |
/* 0370 */ "\x72\x61\x6d\x65\x20\x69\x6e\x20\x66\x69\x78\x65\x64\x20\x6c\x65" //rame in fixed le |
/* 0380 */ "\x6e\x67\x74\x68\x20\x72\x65\x63\x6f\x72\x64\x73\x20\x62\x75\x74" //ngth records but |
/* 0390 */ "\x20\x6f\x6e\x20\x61\x20\x44\x45\x43\x20\x54\x4f\x50\x53\x2d\x32" // on a DEC TOPS-2 |
/* 03a0 */ "\x30\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x61\x73\x20\x61\x20" //0.         as a  |
/* 03b0 */ "\x73\x74\x72\x65\x61\x6d\x20\x6f\x66\x20\x63\x68\x61\x72\x61\x63" //stream of charac |
/* 03c0 */ "\x74\x65\x72\x73\x20\x70\x61\x72\x74\x69\x74\x69\x6f\x6e\x65\x64" //ters partitioned |
/* 03d0 */ "\x20\x69\x6e\x74\x6f\x20\x6c\x69\x6e\x65\x73\x2c\x20\x66\x6f\x72" // into lines, for |
/* 03e0 */ "\x20\x65\x78\x61\x6d\x70\x6c\x65\x0a\x20\x20\x20\x20\x20\x20\x20" // example.        |
/* 03f0 */ "\x20\x20\x62\x79\x20\x3c\x43\x52\x4c\x46\x3e\x2e\x20\x20\x49\x66" //  by <CRLF>.  If |
/* 0400 */ "\x20\x74\x68\x65\x20\x74\x72\x61\x6e\x73\x66\x65\x72\x20\x6f\x66" // the transfer of |
/* 0410 */ "\x20\x66\x69\x6c\x65\x73\x20\x62\x65\x74\x77\x65\x65\x6e\x20\x73" // files between s |
/* 0420 */ "\x75\x63\x68\x20\x64\x69\x73\x70\x61\x72\x61\x74\x65\x0a\x20\x20" //uch disparate.   |
/* 0430 */ "\x20\x20\x20\x20\x20\x20\x20\x73\x69\x74\x65\x73\x20\x69\x73\x20" //       sites is  |
/* 0440 */ "\x74\x6f\x20\x62\x65\x20\x75\x73\x65\x66\x75\x6c\x2c\x20\x74\x68" //to be useful, th |
/* 0450 */ "\x65\x72\x65\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x73\x6f\x6d\x65" //ere must be some |
/* 0460 */ "\x20\x77\x61\x79\x20\x66\x6f\x72\x20\x6f\x6e\x65\x20\x73\x69\x74" // way for one sit |
/* 0470 */ "\x65\x20\x74\x6f\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x72\x65" //e to.         re |
/* 0480 */ "\x63\x6f\x67\x6e\x69\x7a\x65\x20\x74\x68\x65\x20\x6f\x74\x68\x65" //cognize the othe |
/* 0490 */ "\x72\x27\x73\x20\x61\x73\x73\x75\x6d\x70\x74\x69\x6f\x6e\x73\x20" //r's assumptions  |
/* 04a0 */ "\x61\x62\x6f\x75\x74\x20\x74\x68\x65\x20\x66\x69\x6c\x65\x2e\x0a" //about the file.. |
/* 04b0 */ "\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x57\x69\x74\x68\x20\x73" //.         With s |
/* 04c0 */ "\x6f\x6d\x65\x20\x73\x69\x74\x65\x73\x20\x62\x65\x69\x6e\x67\x20" //ome sites being  |
/* 04d0 */ "\x6e\x61\x74\x75\x72\x61\x6c\x6c\x79\x20\x66\x69\x6c\x65\x2d\x6f" //naturally file-o |
/* 04e0 */ "\x72\x69\x65\x6e\x74\x65\x64\x20\x61\x6e\x64\x20\x6f\x74\x68\x65" //riented and othe |
/* 04f0 */ "\x72\x73\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x6e\x61\x74\x75" //rs.         natu |
/* 0500 */ "\x72\x61\x6c\x6c\x79\x20\x72\x65\x63\x6f\x72\x64\x2d\x6f\x72\x69" //rally record-ori |
/* 0510 */ "\x65\x6e\x74\x65\x64\x20\x74\x68\x65\x72\x65\x20\x6d\x61\x79\x20" //ented there may  |
/* 0520 */ "\x62\x65\x20\x70\x72\x6f\x62\x6c\x65\x6d\x73\x20\x69\x66\x20\x61" //be problems if a |
/* 0530 */ "\x20\x66\x69\x6c\x65\x20\x77\x69\x74\x68\x0a\x20\x20\x20\x20\x20" // file with.      |
/* 0540 */ "\x20\x20\x20\x20\x6f\x6e\x65\x20\x73\x74\x72\x75\x63\x74\x75\x72" //    one structur |
/* 0550 */ "\x65\x20\x69\x73\x20\x73\x65\x6e\x74\x20\x74\x6f\x20\x61\x20\x68" //e is sent to a h |
/* 0560 */ "\x6f\x73\x74\x20\x6f\x72\x69\x65\x6e\x74\x65\x64\x20\x74\x6f\x20" //ost oriented to  |
/* 0570 */ "\x74\x68\x65\x20\x6f\x74\x68\x65\x72\x2e\x20\x20\x49\x66\x20\x61" //the other.  If a |
/* 0580 */ "\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x74\x65\x78\x74\x20\x66" //.         text f |
/* 0590 */ "\x69\x6c\x65\x20\x69\x73\x20\x73\x65\x6e\x74\x20\x77\x69\x74\x68" //ile is sent with |
/* 05a0 */ "\x20\x72\x65\x63\x6f\x72\x64\x2d\x73\x74\x72\x75\x63\x74\x75\x72" // record-structur |
/* 05b0 */ "\x65\x20\x74\x6f\x20\x61\x20\x68\x6f\x73\x74\x20\x77\x68\x69\x63" //e to a host whic |
/* 05c0 */ "\x68\x20\x69\x73\x20\x66\x69\x6c\x65\x0a\x20\x20\x20\x20\x20\x20" //h is file.       |
/* 05d0 */ "\x20\x20\x20\x6f\x72\x69\x65\x6e\x74\x65\x64\x2c\x20\x74\x68\x65" //   oriented, the |
/* 05e0 */ "\x6e\x20\x74\x68\x61\x74\x20\x68\x6f\x73\x74\x20\x73\x68\x6f\x75" //n that host shou |
/* 05f0 */ "\x6c\x64\x20\x61\x70\x70\x6c\x79\x20\x61\x6e\x20\x69\x6e\x74\x65" //ld apply an inte |
/* 0600 */ "\x72\x6e\x61\x6c\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x74\x72" //rnal.         tr |
/* 0610 */ "\x61\x6e\x73\x66\x6f\x72\x6d\x61\x74\x69\x6f\x6e\x20\x74\x6f\x20" //ansformation to  |
/* 0620 */ "\x74\x68\x65\x20\x66\x69\x6c\x65\x20\x62\x61\x73\x65\x64\x20\x6f" //the file based o |
/* 0630 */ "\x6e\x20\x74\x68\x65\x20\x72\x65\x63\x6f\x72\x64\x20\x73\x74\x72" //n the record str |
// Sent dumped on target (-1) 1600 bytes |
// RdpdrDriveReadTask::run: NumberOfBytesRead=468 |
// Sending on target (-1) 8 bytes |
/* 0000 */ "\x14\x08\x00\x00\x02\x00\x00\x00"                                 //........ |
// Sent dumped on target (-1) 8 bytes |
// Sending on target (-1) 468 bytes |
/* 0000 */ "\x75\x63\x74\x75\x72\x65\x2e\x0a\x20\x20\x20\x20\x20\x20\x20\x20" //ucture..         |
/* 0010 */ "\x20\x4f\x62\x76\x69\x6f\x75\x73\x6c\x79\x2c\x20\x74\x68\x69\x73" // Obviously, this |
/* 0020 */ "\x20\x74\x72\x61\x6e\x73\x66\x6f\x72\x6d\x61\x74\x69\x6f\x6e\x20" // transformation  |
/* 0030 */ "\x73\x68\x6f\x75\x6c\x64\x20\x62\x65\x20\x75\x73\x65\x66\x75\x6c" //should be useful |
/* 0040 */ "\x2c\x20\x62\x75\x74\x20\x69\x74\x20\x6d\x75\x73\x74\x0a\x20\x20" //, but it must.   |
/* 0050 */ "\x20\x20\x20\x20\x20\x20\x20\x61\x6c\x73\x6f\x20\x62\x65\x20\x69" //       also be i |
/* 0060 */ "\x6e\x76\x65\x72\x74\x69\x62\x6c\x65\x20\x73\x6f\x20\x74\x68\x61" //nvertible so tha |
/* 0070 */ "\x74\x20\x61\x6e\x20\x69\x64\x65\x6e\x74\x69\x63\x61\x6c\x20\x66" //t an identical f |
/* 0080 */ "\x69\x6c\x65\x20\x6d\x61\x79\x20\x62\x65\x20\x72\x65\x74\x72\x69" //ile may be retri |
/* 0090 */ "\x65\x76\x65\x64\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x75\x73" //eved.         us |
/* 00a0 */ "\x69\x6e\x67\x20\x72\x65\x63\x6f\x72\x64\x20\x73\x74\x72\x75\x63" //ing record struc |
/* 00b0 */ "\x74\x75\x72\x65\x2e\x0a\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20" //ture...          |
/* 00c0 */ "\x49\x6e\x20\x74\x68\x65\x20\x63\x61\x73\x65\x20\x6f\x66\x20\x61" //In the case of a |
/* 00d0 */ "\x20\x66\x69\x6c\x65\x20\x62\x65\x69\x6e\x67\x20\x73\x65\x6e\x74" // file being sent |
/* 00e0 */ "\x20\x77\x69\x74\x68\x20\x66\x69\x6c\x65\x2d\x73\x74\x72\x75\x63" // with file-struc |
/* 00f0 */ "\x74\x75\x72\x65\x20\x74\x6f\x20\x61\x0a\x20\x20\x20\x20\x20\x20" //ture to a.       |
/* 0100 */ "\x20\x20\x20\x72\x65\x63\x6f\x72\x64\x2d\x6f\x72\x69\x65\x6e\x74" //   record-orient |
/* 0110 */ "\x65\x64\x20\x68\x6f\x73\x74\x2c\x20\x74\x68\x65\x72\x65\x20\x65" //ed host, there e |
/* 0120 */ "\x78\x69\x73\x74\x73\x20\x74\x68\x65\x20\x71\x75\x65\x73\x74\x69" //xists the questi |
/* 0130 */ "\x6f\x6e\x20\x6f\x66\x20\x77\x68\x61\x74\x0a\x20\x20\x20\x20\x20" //on of what.      |
/* 0140 */ "\x20\x20\x20\x20\x63\x72\x69\x74\x65\x72\x69\x61\x20\x74\x68\x65" //    criteria the |
/* 0150 */ "\x20\x68\x6f\x73\x74\x20\x73\x68\x6f\x75\x6c\x64\x20\x75\x73\x65" // host should use |
/* 0160 */ "\x20\x74\x6f\x20\x64\x69\x76\x69\x64\x65\x20\x74\x68\x65\x20\x66" // to divide the f |
/* 0170 */ "\x69\x6c\x65\x20\x69\x6e\x74\x6f\x20\x72\x65\x63\x6f\x72\x64\x73" //ile into records |
/* 0180 */ "\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x77\x68\x69\x63\x68\x20" //.         which  |
/* 0190 */ "\x63\x61\x6e\x20\x62\x65\x20\x70\x72\x6f\x63\x65\x73\x73\x65\x64" //can be processed |
/* 01a0 */ "\x20\x6c\x6f\x63\x61\x6c\x6c\x79\x2e\x20\x20\x49\x66\x20\x74\x68" // locally.  If th |
/* 01b0 */ "\x69\x73\x20\x64\x69\x76\x69\x73\x69\x6f\x6e\x20\x69\x73\x20\x6e" //is division is n |
/* 01c0 */ "\x65\x63\x65\x73\x73\x61\x72\x79\x2c\x0a\x20\x20\x20\x20\x20\x20" //ecessary,.       |
/* 01d0 */ "\x20\x20\x20\x74"                                                 //   t |
// Sent dumped on target (-1) 468 bytes |
} /* end outdata */;
