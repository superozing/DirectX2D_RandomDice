rmdir /s /q ".\External\Include\Scripts"
mkdir ".\External\Include\Scripts"

xcopy /s /y  /exclude:exclude_list.txt ".\Project\Scripts\*.h" ".\External\Include\Scripts"