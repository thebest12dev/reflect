import os
import requests

def download_files(urls, directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

    for i, url in enumerate(urls):
        try:
            print(f"Downloading file {i + 1} of {len(urls)}: {url}")
            response = requests.get(url, stream=True)
            response.raise_for_status() 
            
            filename = os.path.join(directory, url.split("/")[-1])
            
            with open(filename, "wb") as file:
                for chunk in response.iter_content(chunk_size=8192):
                    file.write(chunk)

        
        except requests.exceptions.RequestException as e:
            print(f"Failed to download {url}: {e}")

if __name__ == "__main__":
    
    file_urls = [
        "https://raw.githubusercontent.com/leethomason/tinyxml2/refs/heads/master/tinyxml2.cpp",
        "https://raw.githubusercontent.com/leethomason/tinyxml2/refs/heads/master/tinyxml2.h",
    ]
    
   
    output_directory = "lib"

    
    download_files(file_urls, output_directory)
