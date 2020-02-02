- init:
    run: rm -f hw3
    blocker: true

- build:
    run: g++ -Wall -Werror main.cpp -o hw3 # timeout: 4
    blocker: true


- case1:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Message:this is a message To:0 5 mh_13[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case2:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5 2 7 9 12 6 16 3[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Message:this is another message To:0 3 mh_8[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case3:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5 2 7 9[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Message:message3 To:0 9 mh_26[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case4:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5 2 7 9 12 6 16 3[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Can not be reached the mobile host mh_120 at the moment[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case5:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5 2 7[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Message:message5 To:0 5 7 mh_27[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case6:
    run: ./hw3 Network.txt Messages.txt
    points: 20
    script:
        - expect: "[ \r\n]*Traversing:0 5 2 7 9 12 6[ \r\n]*"                                                             # timeout: 2
        - expect: "[ \r\n]*Message:message6 To:0 12 6 mh_6[ \r\n]*"                                                             # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0



