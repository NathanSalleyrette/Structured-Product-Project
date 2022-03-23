import React, { useEffect, useState } from "react";
import Chart from "react-apexcharts";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Dropdown from "react-bootstrap/Dropdown";
import {Table, Button } from "react-bootstrap";
import ChartProductForm from "./chartProductForm";


const BACKEND = "http://localhost:5000/api";
//const BACKEND = "https://peps-group1.herokuapp.com//api";

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


const ChartStock = (props) => {

  const [pAndL, setPAndL] = useState();

  const [deltas, setDeltas] = useState([]);

  const [disabled, setDisabled] = useState(false);
  
  async function simpleRates(numberOfRebalancing, startDate, nbMCSamples) {
    setDisabled(true);

    await fetch(`${BACKEND}/simpleRates`, {
      method : "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: `data=${JSON.stringify({ numberOfRebalancing, startDate, nbMCSamples })}`,
    })
    .then((res) => res.json())
    .then((body) => {
      setState( {
        options: {
          ...state.options,
          xaxis: {
            type : "datetime",
             categories: body.date
          },
        },
        series: [
          {
            name : "Product Price",
            data: body.price,
          },
          {
            name : "Portfolio Value",
            data : body.port
          }
        ],
      });
      console.log(body.port)
      setPAndL((body.port[body.port.length-1] - body.price[body.price.length-1])*100/body.price[body.price.length-1])
      setDisabled(false);


    })
    .catch(function (error) {
      console.log("error", error);
      setDisabled(false);
    });
  }

  async function getDeltas(date, nbMCSamples) {
    setDisabled(true);

    await fetch(`${BACKEND}/getDeltas`, {
      method : "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: `data=${JSON.stringify({ date, nbMCSamples })}`,
    })
    .then((res) => res.json())
    .then((body) => {
      setDeltas(body.repData);
      setDisabled(false);


    })
    .catch(function (error) {
      console.log("error", error);
      setDisabled(false);
    });
  }


  function renderDelta(delta, index){
      return (
      <tr key = {index} className = {delta.stockName}>
        <td>{delta.stockName}</td>
        <td>{delta.delta}</td>
      </tr>)
  }


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
          {/*No <Col> here because it's already in ChartForm */}
          <ChartProductForm pAndL={pAndL} simpleRates={simpleRates} getDeltas={getDeltas} buttonDisabled={disabled}/>

      </Row>
      <Row>
        <Col sm="8">
          <div className="chart">
            <div className="app">
              <div className="row">
                <div className="mixed-chart">
                  <Chart className="d-flex justify-center"
                    options={state.options}
                    series={state.series}
                    type="area"
                    width="100%"
                  />
                </div>
              </div>
            </div>
          </div>
        </Col>
        <Col sm="4" style={{height:"800px", overflow:"scroll"}}>
        <Table>
                <thead>
                    <tr>
                        <td>Asset Name</td>
                        <td>Delta</td>
                    </tr>
                </thead>
                <tbody>
                    {deltas.map((delta, index) => renderDelta(delta, index))}
                </tbody>
            </Table></Col>
      </Row>
    </>
  );
};

export default ChartStock;
