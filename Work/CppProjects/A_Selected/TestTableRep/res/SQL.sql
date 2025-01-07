
CREATE TABLE [CompanyRepRes](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[CompanyIDLink] [int] NOT NULL,
	[FontsYear] [int] NOT NULL,
	[ColorsYear] [int] NOT NULL,
	[StringsYear] [int] NOT NULL,
	[ImagesYear] [int] NOT NULL,
	[MemosYear] [int] NOT NULL,
 CONSTRAINT [PK_CompanyRepRes] PRIMARY KEY  
(
	[CompanyID] ASC,
	[Year] ASC
)
);


CREATE TABLE [CompanyStrings](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[ID] [varchar](16) NOT NULL,
	[Value] [nvarchar](4000) NOT NULL,
 CONSTRAINT [PK_CompanyStrings] PRIMARY KEY 
(
	[CompanyID] ASC,
	[Year] ASC,
	[ID] ASC
)
);


CREATE TABLE [CompanyMemos](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[ID] [varchar](16) NOT NULL,
	[Value] [varchar](8000) NOT NULL,
 CONSTRAINT [PK_CompanyMemos] PRIMARY KEY  
(
	[CompanyID] ASC,
	[Year] ASC,
	[ID] ASC
)
);


CREATE TABLE [CompanyImages](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[ID] [varchar](16) NOT NULL,
	[Value] [varbinary] NOT NULL,	
 CONSTRAINT [PK_CompanyImages] PRIMARY KEY  
(
	[CompanyID] ASC,
	[Year] ASC,
	[ID] ASC
)
);


CREATE TABLE [CompanyColors](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[GroupID] [varchar](8) NOT NULL,
	[ID] [varchar](16) NOT NULL,
	[R] [int] NOT NULL,
	[G] [int] NOT NULL,
	[B] [int] NOT NULL,	
	[A] [int] NOT NULL,
 CONSTRAINT [PK_CompanyColors] PRIMARY KEY  
(
	[CompanyID] ASC,
	[Year] ASC,
	[GroupID] ASC,
	[ID] ASC
)
);


CREATE TABLE [CompanyFonts](
	[CompanyID] [int] NOT NULL,
	[Year] [int] NOT NULL,
	[GroupID] [varchar](8) NOT NULL,
	[ID] [varchar](16) NOT NULL,
	[Family] [varchar](64) NOT NULL,
	[Size] [int] NOT NULL,
	[Bold] [char](1) NOT NULL,
	[Italic] [char](1) NOT NULL,
	[Underline] [char](1) NOT NULL,
 CONSTRAINT [PK_CompanyFonts] PRIMARY KEY  
(
	[CompanyID] ASC,
	[Year] ASC,
	[GroupID] ASC,
	[ID] ASC
)
);

