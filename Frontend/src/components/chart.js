import React, { useState } from "react";
import Chart from "react-apexcharts";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Dropdown from "react-bootstrap/Dropdown";
import { Button, FormControl } from "react-bootstrap";
import ChartForm from "./chartForm";

const BACKEND = "http://localhost:5000/api";


const CustomToggle = React.forwardRef(({ children, onClick,  buttonDisabled}, ref) => (
  <Button className="btn btn-primary custom"
    href=""
    ref={ref}
    onClick={(e) => {
      e.preventDefault();
      onClick(e);
    }}
    disabled={buttonDisabled}
  >
    {children}
    &#x25bc;
  </Button>
));

const CustomMenu = React.forwardRef(
  ({ children, style, className, "aria-labelledby": labeledBy }, ref) => {
    const [value, setValue] = useState("");

    return (
      <div
        ref={ref}
        style={style}
        className={className}
        aria-labelledby={labeledBy}
      >
        <FormControl
          autoFocus
          className="mx-3 my-2 w-auto"
          placeholder="Type to filter..."
          onChange={(e) => setValue(e.target.value)}
          value={value}
        />
        <ul className="list-unstyled list-group">
          {React.Children.toArray(children).filter((child) => {
            /* Attention aux valeurs qui ne sont pas des caractères 
            Ne fonctionne pas si les actions comportent des nombres*/
            return !value || child.props.children.startsWith(value);
          })}
        </ul>
      </div>
    );
  }
);

const ChartStock = (props) => {
  const [stock, setStock] = useState({
    stockNoun: "CSX Corporation",
    yahooCode: "CSX",
  });

  const [disabled, setDisabled] = useState(false);
  function handleSelect(e) {
    var uwu = JSON.parse(e);
    setStock(uwu);
  };


  async function getHistoricalPrices(
    frequency,
    startDate,
    callback,
    cors
  ) {
    setDisabled(true);
    // setState({
    //   options: {
    //     noData: {
    //       text: 'Fetching Data ...'
    //     },
    //     xaxis: {
    //       type: "datetime",
    //     categories: [],
    //     },
    //   },
    //   series: [
    //     {
    //       name: "",
  
    //       data: [],
    //     },
    //   ],
    // })
    if (callback === void 0) {
      callback = undefined;
    }
    if (cors === void 0) {
      cors = "no-cors";
    }


    var requestOptions = {
      method: "GET",
      headers: {
        accept: "application/json",
        "Content-Type": "application/x-www-form-urlencoded",
      },
    };
    const url = new URL(`${BACKEND}/yahoo`);
    url.searchParams.set("yahooCode", stock.yahooCode)
    url.searchParams.set("begin_date", Math.floor(new Date(startDate + 'T03:24:00').getTime() / 1000))
    url.searchParams.set("end_date", Math.floor(new Date().getTime() / 1000))
    url.searchParams.set("frequency", frequency)
    await fetch(url, requestOptions)
      .then((response) => response.json())
      .then((body) => {
        console.log(body);
        dates.length = 0;
        stocks.length = 0;
        body.date.map((el) => dates.push(el));
        body.close.map((el) => stocks.push(el));
        // //const seriesdata = body.CSX.timestamp.map((t) => [t, 0]);
        setState( {
          options: {
            ...state.options,
            xaxis: {
              ...state.options.xaxis,
               categories: body.date.map((date) => {let time = new Date(date + 'T03:24:00').getTime()
             return time}),
                //categories: body.date,
            },
          },
          series: [
            {
              ...state.series,
              data: body.close,
            },
          ],
        });

        // setState(prices);
        setDisabled(false);

      })
      .then(() => {
        console.log(dates)
        console.log(stocks)
      })
      .catch(function (error) {
        console.log("error", error);
        setDisabled(false);
      });
  }


  const [listStock, setListStock] = useState({
    data: [
      { yahooCode: "CSX", stockNoun: "CSX Corporation" },
      { yahooCode: "VIE.PA", stockNoun: "Veolia Environnement S.A." },
      { yahooCode: "LIN.DE", stockNoun: "Linde plc" },
    ],
  });


  const dates = [];
  const stocks = [];
  const [state, setState] = useState({
    options: {
      chart: {
        id: "basic-bar",
        zoom: {
          type: 'x',
          enabled: true,
          autoScaleYaxis: true
        },
      },
      xaxis: {
        type: "datetime",
        categories: [],
      },
      dataLabels: {
        enabled: false
      },
      markers: {
        size: 0,
      },
      title: {
        text: 'Stock Price Movement',
        align: 'left'
      },
      fill: {
        type: 'gradient',
        gradient: {
          shadeIntensity: 1,
          inverseColors: false,
          opacityFrom: 0.5,
          opacityTo: 0,
          stops: [0, 90, 100]
        },
      },
      noData: {
        text: 'Fetching data'
      }
      
    },

    series: [
      {
        name: "Close",

        data: [],
      },
    ],
  });

  
  return (
    <>
      {" "}
      <Row>
        {/* If more than 12 columns are placed within a single row, each group of extra columns will, as one unit, wrap onto a new line. */}


        <Col>
          <Dropdown onSelect={handleSelect}>
            <Dropdown.Toggle as={CustomToggle} id="dropdown-custom-components" buttonDisabled={disabled}>
              {stock.stockNoun}
            </Dropdown.Toggle>

            <Dropdown.Menu as={CustomMenu}>
              {listStock.data.map((d) => (
                <Dropdown.Item key={d.yahooCode} eventKey={JSON.stringify(d)}>
                  {d.stockNoun}
                </Dropdown.Item>
              ))}
            </Dropdown.Menu>
          </Dropdown>
        </Col>
          {/*No <Col> here because it's already in ChartForm */}
          <ChartForm  onConnect={getHistoricalPrices} buttonDisabled={disabled}/>

      </Row>
      <Row>
        <Col>
          <div className="chart">
            <div className="app">
              <div className="row">
                <div className="mixed-chart">
                  <Chart className="d-flex justify-center"
                    options={state.options}
                    series={state.series}
                    type="area"
                    width="70%"
                  />
                </div>
              </div>
            </div>
          </div>
        </Col>
      </Row>
    </>
  );
};

export default ChartStock;
