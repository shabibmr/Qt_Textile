#ifndef CONFIGSETTINGSVARIABLES_H
#define CONFIGSETTINGSVARIABLES_H


#include <QtGlobal>


// ALL THE GLOBAL DECLARATIONS

// don't use #include <QString> here, instead do this:

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

// that way you aren't compiling QString into every header file you put this in...
// aka faster build times.

#define MAGIC_NUM 42

extern qreal g_some_double; // Note the important use of extern!
extern const QString dbverison;
extern const QString useBom;
extern const QString askForPrint;
extern const QString NOKOTPRINT;
extern const QString printKOtfromSO;
extern const QString tokenSystem;
extern const QString defaultServiceMode;
extern const QString TakeawayCount;
extern const QString testKey;
extern const QString saveOnRefresh;
extern const QString askAddress;
extern const QString NeverAskAddress;
extern const QString showCustomerNameinPOS;

extern const QString kotFormat;
extern const QString kotItemsFontSize;
extern const QString refString;
extern const QString refStringSize;
extern const QString kotAddrPrintSize;

extern const QString printAddressInKot;
extern const QString address1Route2;
extern const QString noDefaultKotPrint;

extern const QString timePrintSize;
extern const QString kotCompanyName;
extern const QString kotService;
//extern const QString kotCompanyName;


extern const QString showAdj;
extern const QString showPlusFive;
extern const QString plusFiveValue;
extern const QString POSLang;
extern const QString showDarktheme;
extern const QString showDraftedShortcuts;


extern const QString taxName;
extern const QString showDiscount;
extern const QString showTaxinVoucher;
extern const QString voucherEditorItemsColWidth;
extern const QString showBarcodeInVoucher;
extern const QString showMrpinVoucher;
extern const QString askWhichPrinterSales;
extern const QString printVoucherPref;
extern const QString printSimple;
extern const QString printBarcodeInBill;


extern const QString posColCount;
extern const QString posButtonWidth;
extern const QString posButtonHeight;

extern const QString tablesViewColCount;
extern const QString TimeSetManuallyPOS;

extern const QString disableItemEditPOS;
extern const QString allowNegativeBilling;
extern const QString allowItemDiscinPOS;

extern const QString leftwords;


extern const QString showBillwiseMapping;
//extern const QString masterDbName;
extern const QString promptToSendToOtherLedgers;
extern const QString shiftCashLimit;

extern const QString POSDrafttableHideCol;

extern const QString POSallowSOCopy;
extern const QString POSshowLedger;

extern const QString tabBillPrinter;
extern const QString PluDirectory;
extern const QString showPrintOnOff;
extern const QString printFormat;
extern const QString multiVoucherFlag;
extern const QString showStockAdjInReport;
extern const QString showStockReportInShift;
extern const QString checkShift;
extern const QString inventoryImageUploadURL;
extern const QString showRouteFilterInDrafts;
extern const QString showAllDrafts;
extern const QString focusOnCreateNew;
extern const QString showAccountingReports;
extern const QString promptItemChecklist;
extern const QString allowSaveWithoutChecklist;
extern const QString printCancelledKOT;
extern const QString deleteKOTItemWithoutRemarks;
extern const QString ServeOnlyPrefix;
extern const QString autoAllotRefTable;

extern const QString hideOpenTillButton;
extern const QString shiftNoPendingDraft;
extern const QString showPaymentsInEmail;
extern const QString customerscreenfontsize;
extern const QString printBillLineAmountWithTax;
extern const QString printBillLinePriceWithTax;

extern const QString printItemNarrationinSO;
extern const QString printItemNarrationinBill;






extern const QString billPreviewWidth;
extern const QString useDefaultItemSection;
extern const QString allowItemMultipleEntry;

extern const QString autoOpenTill;

extern const QString strictFilter;
extern const QString showPrefixInRegister;
extern const QString RefPrintSize;
extern const QString billAddrPrintSize;
extern const QString kotNarrationTitle;
extern const QString companyId;

extern const QString tableGridFontSize;
extern const QString tableGridButtonWidth;
extern const QString tableGridButtonHeight;

extern const QString debug;

extern const QString showChequeEntry;
extern const QString showPriceListForSaveVoucher;
extern const QString openSerialPortCnf;

extern const QString hideSalesOrderPrintButton;
extern const QString minwidPOSTableWidth;
extern const QString pOSIconSize;
extern const QString pOSIconWidth;
extern const QString pOSIconHeight;

extern const QString showPOS;
extern const QString checkDNexists ;
extern const QString allowEmptyAddress;
extern const QString showBillwiseInLedgerReport;
extern const QString printBarcode3020;
extern const QString barcodeSpacing;
extern const QString showLedgerwise;
extern const QString barcodeFontSize;
extern const QString barcodeLetterSpacing ;
extern const QString barcodeStickerHeight ;
extern const QString barcodeStickerWidth ;
extern const QString barcodeStickerFamily ;
extern const QString barcodeCount;
extern const QString barcodeMiddleMargin;
extern const QString barcodeFSSAI;
extern const QString bankDetailsPrintTop;
extern const QString salesVoucherPrefix;
extern const QString printRateWithTax;
extern const QString printItemTotalWithTax;
extern const QString showCashOutButton;
extern const QString updatePriceByLedger ;
extern const QString printHSN;
extern const QString customerFontSize;
extern const QString printDriverDetails;
extern const QString draftsHours ;
extern const QString showDateFiltersInReports;
extern const QString openNewEditorOnSave;
extern const QString showTaxColInPrint;
extern const QString updateItemPriceByBatch;
extern const QString hideDiscColInPrint;
extern const QString printMRP;
extern const QString allowedDiscounts;
extern const QString hideComplimentaryButton;
extern const QString hideLoyalityButton;
extern const QString emailsmtp;




extern const QString scaleWidthLogo ;
extern const QString scaleHeightLogo ;
extern const QString borderMarginLogo ;
extern const QString logoMarginHeight;
extern const QString printRateInDN ;
extern const QString stopByCreditLimit;
extern const QString mailPwd;
extern const QString showReorderPopup;
extern const QString closingStockPrint;
extern const QString addShippingInfo;
extern const QString showItemLabelinfo;
extern const QString showPOSButton;
extern const QString showProductionModule;
extern const QString isHotel;
extern const QString itemMasterQuery;
extern const QString showShortcuts;
extern const QString reportSortEnabled;
extern const QString updateConfigDb;
extern const QString allItemsStockMonitored;
extern const QString allItemsPurchaseItem;
extern const QString allItemsSalesItem;
extern const QString salesPrintFontSize;

extern const QString showJobOrderManual;




extern const QString QRPrintWidth;
extern const QString QRLeftMargin;
extern const QString QRTopMargin;
extern const QString QRCodeLeftMargin;
extern const QString QRCodeTopMargin;


extern const QString QRStickerLengthMM;
extern const QString QRStickerHeightMM;
extern const QString QRStickerMiddleMarginMM;
extern const QString QRStickerGapMM;

extern const QString QRStickerWidth;
extern const QString stockValuationType;
extern const QString gstSalesMultipleRows;
extern const QString tableRowHeight;

extern const QString shiftOpeningCash;
extern const QString showVarianceStock;


class ConfigSettingsVariables
{
public:
    ConfigSettingsVariables();
    static const QString DB_Version;

};

#endif // CONFIGSETTINGSVARIABLES_H
