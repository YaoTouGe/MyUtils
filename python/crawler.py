from importlib.resources import path
import urllib3
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.common.by import By
import time
import subprocess
import os

# https://www.libertylondon.com/uk/department/fabric/cotton/cotton-tana-lawn/?start=0&sz=421
# curl -x 127.0.0.1:9910 -o test.webp https://i8.amplience.net/i/liberty/000742589-R428318006-1?$hi-res$&fmt=auto

# http = urllib3.ProxyManager("http://127.0.0.1:9910")
# r = http.request(
#     'GET', "https://www.libertylondon.com/uk/department/fabric/cotton/cotton-tana-lawn/?start=0&sz=36")
# print(r.status)
# print(r.data)

def AppendEntryUrls(baseUrl, entryUrls, browser):
    browser.get(baseUrl)
    # print(len(browser.page_source))
    entries = browser.find_elements(by=By.CLASS_NAME, value="image-container")

    for entry in entries:
        href = entry.find_element(by=By.TAG_NAME, value="a")
        entryUrls.append(href.get_attribute("href"))

folderNames = ["Organic Cotton", "Silk Satin Fabric", "Silk Crepe Dde Chine Fabrics"]
baseUrls = ["https://www.libertylondon.com/uk/department/fabrics/cotton/organic-cotton/?cgid=fabrics_cotton_Organic-Cotton&start=0&sz=39&srule=undefined",
"https://www.libertylondon.com/uk/department/fabric/silk/silk-satin/?cgid=fabrics_silk_silk-satin&start=0&sz=115&srule=undefined",
"https://www.libertylondon.com/uk/department/fabric/silk/crepe-de-chine/?cgid=fabrics_silk_crepe-de-chine&start=0&sz=107&srule=undefined"]

browser = webdriver.Edge()

for baseIdx in range(len(baseUrls)):
    baseUrl = baseUrls[baseIdx]
    entryUrls = []
    AppendEntryUrls(baseUrl, entryUrls, browser)
    print("total: ", len(entryUrls))

    dict = {}
    for idx in range(len(entryUrls)):
        url = entryUrls[idx]
        browser.get(url)
        # get product name
        productName = browser.find_element(by=By.TAG_NAME, value="h1").get_attribute("textContent")
        productName = productName.replace("™", "_")
        if not productName in dict.keys():
            dict[productName] = 1
        else:
            dict[productName] += 1
            productName = productName + "(" +str(dict[productName]) + ")"
        
        # check path doesn't need escape and quotation
        checkPath = "./laopo/" + productName + str(1) + ".webp"
        if os.path.isfile(checkPath):
            continue

        print(productName)

        primaryImage = browser.find_element(by=By.CLASS_NAME, value="primary-product-image")
        href = primaryImage.find_element(by=By.TAG_NAME, value="a")
        resLink = href.get_attribute("href")

        for i in range(6):
            outputPath = "\"./laopo/%s/"%(folderNames[baseIdx]) + productName + str(i+1) + ".webp\""
            if i > 0:
                resLink = resLink.replace("-%d?" % (i), "-%d?" % (i + 1))
            cmd = "curl -x 127.0.0.1:9910 -o  " + outputPath + " \"" + resLink +"\""
            print(cmd)
            process = subprocess.run(cmd, shell=True)
        
        time.sleep(1)
    
browser.close()
