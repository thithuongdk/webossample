*** Settings ***
Documentation          This example demonstrates executing commands on a remote machine
...                    and getting their output and the return code.
...
...                    Notice how connections are handled as part of the suite setup and
...                    teardown. This saves some time when executing several test cases.

Library                String
Library                Process
Library                Collections
Library                SSHLibrary
Suite Setup            Open Connection And Log In
Suite Teardown         Close Connection

*** Variables ***
${HOST}                localhost
${PORT}                6622
${USERNAME}            root
${PASSWORD}            ${EMPTY}

*** Keywords ***
Open Connection And Log In
    Open Connection     ${HOST}           port=${PORT}
    Login               ${USERNAME}       ${PASSWORD}
Luna Send
    [Arguments]         ${count}          ${command}              ${payload}
    ${responses}=       Execute Command   luna-send -n ${count} -f luna://${command} '${payload}'       timeout=5         output_if_timeout=TRUE
    ${responses} =      Replace String    ${responses}            }\n{            },{
    ${output} =         Evaluate          json.loads("["+$responses+"]")   
Luna Subcribe
    [Arguments]         ${count}          ${command}              ${payload}
    ${responses}=       Execute Command   luna-send -n ${count} -f luna://${command} '${payload}'       timeout=5         output_if_timeout=TRUE
    ${responses} =      Replace String    ${responses}            }\n{            },{
    ${output} =         Evaluate          json.loads("["+$responses+"]")         json

*** Test Cases ***
Execute Luna Test
    [Documentation]    Execute Command luna send 2 response.
    ...                com.webos.service.mediaindexer/getDeviceList.
    ${output}=         Luna Send      1       com.webos.media/getActivePipelines      {}
    ${mediaId}=        Get From Dictionary    ${output}[0][0]    mediaId
    ${output}=         Luna Send    5       com.webos.media/subscribe      {"mediaId":"${mediaId}"}
    Log    The value of the variable is: ${output}
