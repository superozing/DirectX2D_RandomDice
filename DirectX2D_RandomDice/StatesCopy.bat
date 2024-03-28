del /q /f ".\External\Include\States\*.*"
xcopy /s /y  /exclude:exclude_list.txt ".\Project\States\*.h" ".\External\Include\States"