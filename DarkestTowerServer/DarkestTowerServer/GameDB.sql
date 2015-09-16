use Hmmo
go

SET NOCOUNT ON
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

IF EXISTS ( select * from sys.tables where name='AccountTable' )
	DROP TABLE [dbo].[AccountTable]
GO


CREATE TABLE [dbo].[AccountTable](
	[playerUID] [int] NOT NULL PRIMARY KEY IDENTITY(100, 1),
	[name] [nvarchar](32) NOT NULL DEFAULT (N'noname'),
	[password] [nvarchar] (32) NOT NULL,
	[win] [int] NOT NULL DEFAULT ((0)),
	[lose] [int] NOT NULL DEFAULT ((0)),
	[elo] [int] NOT NULL DEFAULT ((0)),
	[createTime] [datetime] NOT NULL,
	[isValid] [tinyint] NOT NULL,
	[comment] [nvarchar](256) NULL
)

GO

IF EXISTS ( select * from sys.tables where name='PlayerHeros' )
	DROP TABLE [dbo].[PlayerHeros]
GO

CREATE TABLE [dbo].[PlayerHeros](
	[heroId] [int] NOT NULL PRIMARY KEY IDENTITY(0, 1),
	[playerId] [int] NOT NULL,
	[heroType] [int] NOT NULL,
	[level] [int] NOT NULL,
	[maxHP] [int] NOT NULL,
	[hp] [int] NOT NULL,
	[maxAP] [int] NOT NULL,
	[ap] [int] NOT NULL
)
GO

IF EXISTS ( select * from sys.tables where name='heroSkills' )
	DROP TABLE [dbo].[heroSkills]
GO

CREATE TABLE [dbo].[heroSkills](
	[heroId] [int] NOT NULL,
	[skillType] [int] NOT NULL,
	[level] [int] NOT NULL
)

GO

-- info 관련 정보들

IF NOT EXISTS ( select * from sys.tables where name='heroData' )
	CREATE TABLE [dbo].[heroData](
		[heroType] [int] NOT NULL PRIMARY KEY,
		[hp] [int] NOT NULL,
		[hpGrow] [int] NOT NULL,
		[ap] [int] NOT NULL,
		[apGrow] [int] NOT NULL
	)
GO

IF NOT EXISTS ( select * from sys.tables where name='skillData' )
	CREATE TABLE [dbo].[skillData](
		[skillType] [int] NOT NULL PRIMARY KEY,
		[heroType] [int] NOT NULL,
		[value] [int] NOT NULL,
		[act] [int] NOT NULL,
		[cool] [int] NOT NULL
	)
GO



IF EXISTS ( select * from sys.procedures where name='spCreateAccount' )
	DROP PROCEDURE [dbo].[spCreateAccount]
GO

CREATE PROCEDURE [dbo].[spCreateAccount]
	@name	NVARCHAR(32),
	@password NVARCHAR(32)
AS
BEGIN
	SET NOCOUNT ON
	IF EXISTS ( SELECT * FROM AccountTable WHERE name = @name)
	BEGIN	
		SELECT 0
	END
	ELSE
	BEGIN	
		INSERT INTO AccountTable([name], [password], [createTime], [win], [lose], [isValid]) VALUES (@name, @password, GETDATE(), 0, 0, 1)
		SELECT @@IDENTITY
	END
END
GO

IF EXISTS ( select * from sys.procedures where name='spDeleteAccount' )
	DROP PROCEDURE [dbo].[spDeleteAccount]
GO

CREATE PROCEDURE [dbo].[spDeleteAccount]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	DELETE FROM AccountTable WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spPlayerWin' )
	DROP PROCEDURE [dbo].[spPlayerWin]
GO

CREATE PROCEDURE [dbo].[spPlayerWin]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET win = win + 1 WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spPlayerLose' )
	DROP PROCEDURE [dbo].[spPlayerLose]
GO

CREATE PROCEDURE [dbo].[spPlayerLose]
	@playerUID	INT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET lose = lose + 1 WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO

IF EXISTS ( select * from sys.procedures where name='spUpdatePlayerValid' )
	DROP PROCEDURE [dbo].[spUpdatePlayerValid]
GO

CREATE PROCEDURE [dbo].[spUpdatePlayerValid]
	@playerUID	INT,
	@valid		TINYINT
AS
BEGIN
	SET NOCOUNT ON
	UPDATE AccountTable SET isValid=@valid WHERE playerUID=@playerUID
	SELECT @@ROWCOUNT
END
GO


IF EXISTS ( select * from sys.procedures where name='spLoadPlayer' )
	DROP PROCEDURE [dbo].[spLoadPlayer]
GO

CREATE PROCEDURE [dbo].[spLoadPlayer]
	@name	NVARCHAR(255),
	@password NVARCHAR(255)
AS
BEGIN
	SET NOCOUNT ON
	SELECT [playerUID], [win], [lose], [elo], [isValid], [comment] FROM AccountTable WHERE [name] = @name AND [password]=@password AND isValid = 1
END		   
GO

IF EXISTS ( select * from sys.procedures where name='spLoadHeroData' )
	DROP PROCEDURE [dbo].[spLoadHeroData]
GO

CREATE PROCEDURE [dbo].[spLoadHeroData]
	@heroType	INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [hp], [hpGrow], [ap], [apGrow] FROM HeroData WHERE heroType=@heroType
END
GO

IF EXISTS ( select * from sys.procedures where name='spLoadSkillData' )
	DROP PROCEDURE [dbo].[spLoadSkillData]
GO

CREATE PROCEDURE [dbo].[spLoadSkillData]
	@skillType	INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT [heroType], [value], [act], [cool] FROM SkillData WHERE skillType=@skillType
END	   
GO

-- test용 계정 생성
EXEC spCreateAccount 'test0', '12345'
EXEC spCreateAccount 'test1', '12345'
EXEC spCreateAccount 'test2', '12345'
EXEC spCreateAccount 'test3', '12345'
EXEC spCreateAccount 'test4', '12345'
